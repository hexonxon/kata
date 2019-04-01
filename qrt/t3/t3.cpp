#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <vector>
#include <queue>
#include <list>
#include <memory>
#include <stdexcept>

#include <pthread.h>

#define LOG_DEBUG(fmt, ...) fprintf(stderr, "%s: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

/**
 * Increment value atomically and return incremented copy
 */
template <typename T>
static T atomic_inc_fetch(T& val)
{
#ifdef __GNUC__
    return __sync_add_and_fetch(&val, 1);
#else
#   error Implement me
#endif
}

/**
 * Simple RAII lock guard (we can't have C++11)
 */
class LockGuard
{
public:
    explicit LockGuard(pthread_mutex_t* lockptr) : m_lockptr(lockptr)
    {
        pthread_mutex_lock(m_lockptr);
    }

    ~LockGuard()
    {
        pthread_mutex_unlock(m_lockptr);
    }

private:
    LockGuard(const LockGuard&) {}
    LockGuard& operator= (const LockGuard&) { return *this; }

    pthread_mutex_t* m_lockptr;
};

struct IReducable
{
    virtual ~IReducable() {};
    virtual IReducable* Process() = 0;
};

////////////////////////////////////////////////////////////////////////////////

template<typename T> struct ReduceTask;
template<typename T> struct ReduceWork;

/**
 * Reduce task subarray.
 *
 * We split taks into levels of subarrays, each subarray handled by a single worker thread.
 * Subarrays are inactive (i.e. are still constructed by their children) until they are put into work queue.
 */
template<typename T>
struct ReduceWork : public IReducable
{
    /* Task context */
    ReduceTask<T>* task;

    /* Subarray slice we work on */
    std::vector<const T*> pdata;

    /* Reduced result */
    T res;

    /* Dependant subarray.
     * If NULL we are the topmost level and will call work completion callback once done. */
    ReduceWork<T>* dependant;

    /* We are dependant subarray, count how many items have been completed so far.
     * When completed_count == count we become reducable. */
    size_t completed_count;

    size_t Size() const
    {
        return pdata.size();
    }

    /* Returns either dependant work to handle next or NULL */
    IReducable* Process()
    {
        ReduceWork<T>* self = dynamic_cast<ReduceWork<T>*>(this);
        ReduceTask<T>* task = self->task;

        /* Reduce my part of the problem */
        self->res = *self->pdata[0];
        for (size_t i = 1; i < self->Size(); ++i) {
            self->res = task->op(self->res, *self->pdata[i]);
        }

        if (self->dependant) {
            /* Atomically increment dependant count and see if we're the last worker to do so.
             * In so then case we can proceed to handle completed subarray ourselves. */
            if (atomic_inc_fetch(self->dependant->completed_count) == self->dependant->Size()) {
                return self->dependant;
            }

            /* We're not the last item in dependant work - leave that to whomever completes last */
        } else {
            /* We are completing overall reduce task */
            task->cb(self->res);
            delete task;
        }

        return NULL;
    }
};

/**
 * Reduce task context.
 */
template<typename T>
struct ReduceTask
{
    typedef T(*ReduceOp)(T, T);
    typedef void(*CompletionCallback)(T);
    typedef ReduceWork<T> work_type;

    /* Initial data */
    ReduceOp op;
    CompletionCallback cb;

    /* Caller-provided data */
    const std::vector<T>* values;

    /* All subarray reduces we need to perform */
    std::vector<work_type> works;

    ReduceTask(ReduceOp op, CompletionCallback cb) : op(op), cb(cb) {}

    std::vector<IReducable*> Decompose(const std::vector<T>* data, size_t max_split)
    {
        std::vector<IReducable*> res;

        /* Compute number of works needed */
        size_t nelements = data->size();
        size_t total_levels = 0;
        size_t total_works = 0;
        
        do {
            size_t nworks = nelements / max_split + (nelements % max_split != 0);
            total_works += nworks;
            total_levels += 1;

            nelements = nworks;
        } while (nelements != 1);

        /* Pre-allocate everything we need for calculated subarray count */
        this->works.resize(total_works);

        /* Allocate space for initial data and intermediate values
         * (don't need space for last element, which is the overall result) */
        this->values = data;

        /* Init works hierarchy */
        nelements = data->size();
        const T* pvalues = this->values->data();
        work_type* pwork = this->works.data();

        for (size_t level = 0; level < total_levels; ++level) {
            size_t nworks = nelements / max_split + (nelements % max_split != 0);
            work_type* prev_work = pwork - nelements;

            /* Init this level of works */
            for (size_t i = 0; i < nworks; ++i, ++pwork) {
                pwork->task = this;
                pwork->dependant = NULL;

                size_t work_size = std::min(nelements - i * max_split, max_split);
                pwork->pdata.resize(work_size, NULL);

                if (level != 0) {
                    /* Set this work as dependable on previous level's works */
                    for (size_t k = 0; k < work_size; ++k, ++prev_work) {
                        prev_work->dependant = pwork;
                        pwork->pdata[k] = &prev_work->res;
                    }
                } else {
                    for (size_t k = 0; k < work_size; ++k) {
                        pwork->pdata[k] = pvalues + k;
                    }

                    /* Save to initial works for reduce pool */
                    res.push_back(pwork);
                    pvalues += work_size;
                }
            }

            nelements = nworks;
        }

        return res;
    }
};

/**
 * Thread pool for reduce tasks
 */
class ReducePool
{
public:

    explicit ReducePool(unsigned n) : m_workers(n), m_should_stop(false)
    {
        if (n == 0) {
            throw new std::invalid_argument("Number of workers should not be 0");
        }

        pthread_mutex_init(&m_queue_lock, NULL);
        pthread_cond_init(&m_queue_cond, NULL);

        for(unsigned i = 0; i < n; ++i) {
            int res = pthread_create(&m_workers[i], NULL, ReducePool::ThreadFunc, this);
            if (res != 0) {
                throw new std::runtime_error("Failed to create worker thread");
            }
        }
    }

    ~ReducePool()
    {
        Stop();

        pthread_mutex_destroy(&m_queue_lock);
        pthread_cond_destroy(&m_queue_cond);
    }

    template<typename T>
    void Enqueue(
        const std::vector<T>* array,
        typename ReduceTask<T>::ReduceOp op,
        size_t max_split,
        typename ReduceTask<T>::CompletionCallback cb)
    {
        if (__atomic_load_n(&m_should_stop, __ATOMIC_ACQUIRE)) {
            return;
        }

        /* Create new work. It will be deleted upon completion */
        ReduceTask<T>* task = new ReduceTask<T>(op, cb);
        EnqueueItems(task->Decompose(array, max_split));
        SignalWork();
    }

    void Stop()
    {
        /* Lock queue to prevent racing with pthread_cond_wait() in WaitNextWork() */
        pthread_mutex_lock(&m_queue_lock);
        __atomic_store_n(&m_should_stop, true, __ATOMIC_RELEASE);
        pthread_mutex_unlock(&m_queue_lock);

        /* Wake workers to complete processing */
        SignalWork();
        for (std::vector<pthread_t>::iterator i = m_workers.begin(); i != m_workers.end(); ++i) {
            pthread_join(*i, NULL);
        }
    }

protected:

    void EnqueueItems(const std::vector<IReducable*>& items)
    {
        LockGuard lock(&m_queue_lock);
        for (std::vector<IReducable*>::const_iterator i = items.begin(); i != items.end(); ++i) {
            m_queue.push(*i);
        }
    }

    void SignalWork()
    {
        pthread_cond_broadcast(&m_queue_cond);
    }

    IReducable* WaitNextWork()
    {
        pthread_mutex_lock(&m_queue_lock);
        while (m_queue.empty()) {
            if (__atomic_load_n(&m_should_stop, __ATOMIC_ACQUIRE)) {
                pthread_mutex_unlock(&m_queue_lock);
                return NULL;
            }
            pthread_cond_wait(&m_queue_cond, &m_queue_lock);
        }

        IReducable* work = m_queue.front();
        m_queue.pop();

        pthread_mutex_unlock(&m_queue_lock);
        return work;
    }

    void ProcessWork()
    {
        IReducable* work = NULL;
        while ((work = WaitNextWork()) != NULL) {
            /* After processing this work item we might be asked to complete its next reduced item.
             * Handle subsequent work until we run out of it. */
            while (work != NULL) {
                work = work->Process();
            }
        }
    }

    static void* ThreadFunc(void* arg)
    {
        ReducePool* self = (ReducePool*) arg;
        self->ProcessWork();
        return NULL;
    }

private:

    std::vector<pthread_t> m_workers;
    std::queue<IReducable*> m_queue;
    pthread_mutex_t m_queue_lock;
    pthread_cond_t m_queue_cond;
    bool m_should_stop;
};

////////////////////////////////////////////////////////////////////////////////

static const int vals[11] = {10, 20, 30, 40, 50, 66, 70, 80, 90, 100, 110};

int sum(int a, int b)
{
    return a + b;
}

long suml(long a, long b)
{
    return a + b;
}

void completion(int v)
{
    LOG_DEBUG("reduced to %d", v);
    assert(v == 666);
}

void completionl(long v)
{
    LOG_DEBUG("reduced to %li", v);
    assert(v == 666);
}

int main(void)
{
    std::vector<int> data(vals, vals + 11);
    std::vector<long> datal(vals, vals + 11);

    {
        ReducePool pool(4);

        /* 3 workers will take 3 subarray, 1 worker is free */
        pool.Enqueue(&data, sum, 4, completion);

        /* 3 workers should be still busy, 1 worker free worker will take first subarray */
        pool.Enqueue(&datal, suml, 3, completionl);
    }

    return EXIT_SUCCESS;
}
