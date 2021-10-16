/**
 * Code that demonstrates spinlock implementations on x86.
 *
 * C11/C++11 memory ordering semantics are delibirately avoided and reimplemented for x86 for the sake
 * of the excercise (although real-life code should use those language models instead for sure).
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_CPUS        4
#define CACHE_ALIGNED   alignas(64)

/* Force compiler to not combine or omit loads of a variable */
#define READ_ONCE(val)  (*(const volatile typeof(val)*)&(val))

/* Complier barrier */
static inline void compiler_barrier()
{
    asm volatile ("":::"memory");
}

/* Atomic load operation.
 *
 * Noop on x86, that is we assume an unaligned 16-, 32- or 64-bit access to cached memory
 * that fit within a cache line (but we don't validate that assumption here).
 *
 * Does not emit any architectural or compiler barriers. */
template <typename T>
static inline T atomic_load(const volatile T *addr)
{
    return *addr;
}

/* Atomic store operation.
 *
 * Noop on x86, that is we assume an unaligned 16-, 32- or 64-bit access to cached memory
 * that fit within a cache line (but we don't validate that assumption here).
 *
 * Does not emit any architectural or compiler barriers. */
template <typename T>
static inline void atomic_store(volatile T *addr, T val)
{
    *addr = val;
}

/* Acquire barrier to prevent load/load and load/store reorders.
 * A noop on x86 */
static inline void acquire_barrier()
{
    /* A load/load + load/store barrier would be required here for non-x86 */
    compiler_barrier();
}

/* Release barrier to prevent load/store and store/store reorders.
 * A noop on x86 */
static inline void release_barrier()
{
    /* A load/store + store/store barrier would be required here for non-x86 */
    compiler_barrier();
}

/* A read-acquire operation that combines (atomic) load with an acquire barrier */
template <typename T>
static inline void read_acquire(const volatile T *addr)
{
    T ret = atomic_load(addr);
    acquire_barrier();
    return ret;
}

/* A write-release operation that combines (atomic) store with a release barrier */
template <typename T>
static inline void write_release(volatile T *addr, T val)
{
    release_barrier();
    atomic_store(addr, val);
}

/* Atomic + full barrier test and set operation.
 * Sets *addr to true and returns previous value. */
template <typename T>
static T test_and_set(volatile T *addr)
{
    T ret;
    asm volatile (
        "mov    $1, %%rax \r\n"
        "xchg   %1, %0 \r\n"
        : "+m"(*addr), "=a"(ret)
        :
        : "memory" /* compiler barrier */
    );

    return ret;
}

/* Atomic + full barrier compare-exchange operation.
 * Sets *addr to val if current value equals to old. Return previous contents of *addr. */
template <typename T>
static T compare_and_swap(volatile T *addr, T old, T val)
{
    asm volatile (
        "lock cmpxchg  %2, %1 \r\n"
        : "+a"(old), "+m"(*addr)
        : "r"(val)
        : "memory" /* compiler barrier */
    );

    return old;
}

/* Atomic + full barrier fetch and increment operation.
 * Return previous contents of *addr */
template <typename T>
static T fetch_and_inc(volatile T *addr)
{
    T val = 1;
    asm volatile (
        "lock xadd  %0, %1 \r\n"
        : "+r"(val), "+m"(*addr)
        :
        : "memory" /* compiler barrier */
    );

    return val;
}

/* Atomic + full barrier fetch and store operation.
 * Returns previous contents of *addr. */
template <typename T>
static T fetch_and_store(volatile T *addr, T val)
{
    asm volatile (
        "xchg   %1, %0 \r\n"
        :"+m"(*addr), "+a"(val)
        :
        : "memory" /* compiler barrier */
    );

    return val;
}


/*
 * Exponential backoff delay spinlock implementation.
 *
 * Unfair implementation that generates some significant contention on the cpu bus.
 * Spins on the shared variable in cache and the goes out for an atomic xchg.
 */

struct CACHE_ALIGNED delay_lock
{
    int lock;
};

void spin_init(struct delay_lock *l)
{
    l->lock = 0;
}

void spin_lock(struct delay_lock *l)
{
    unsigned long delay_us = 1;
    while (atomic_load(&l->lock) || test_and_set(&l->lock)) {
        usleep(delay_us);
        delay_us *= 2;
    }

    /* Acquire barrier is not necessary here, becase we finish locking with a full barrier (test_and_set)
     * anyway, but keep it for demostration purposes. */
    acquire_barrier();
}

void spin_unlock(struct delay_lock *l)
{
    write_release(&l->lock, 0);
}


/*
 * Ticket lock
 *
 * Classic fair implementation.
 * Generates some bus contention by spinning on a shared variable.
 */

struct CACHE_ALIGNED ticket_lock
{
    unsigned long now;
    unsigned long next;
};

void spin_init(struct ticket_lock *l)
{
    l->now = 0;
    l->next = 0;
}

void spin_lock(struct ticket_lock *l)
{
    unsigned long ticket = fetch_and_inc(&l->next);
    while (atomic_load(&l->now) != ticket)
        ;

    acquire_barrier();
}

void spin_unlock(struct ticket_lock *l)
{
    write_release(&l->now, l->now + 1);
}


/*
 * Array-based queueing lock
 *
 * Optimal fair implementation where every CPU gets its own variable to spin on.
 * Although requires O(MAX_CPUS) space.
 */

struct CACHE_ALIGNED array_queue_node
{
    int flag;
};

struct CACHE_ALIGNED array_queue_lock
{
    struct array_queue_node queue[MAX_CPUS];
    CACHE_ALIGNED unsigned long head;
    CACHE_ALIGNED unsigned long tail;
};

void spin_init(struct array_queue_lock *l)
{
    l->tail = 0;
    l->head = 0;

    memset(l->queue, 0, sizeof(l->queue));
    l->queue[0].flag = 1;
}

void spin_lock(struct array_queue_lock *l)
{
    unsigned long pos = fetch_and_inc(&l->tail) % MAX_CPUS;
    while (atomic_load(&l->queue[pos].flag) != 1)
        ;

    acquire_barrier();
}

void spin_unlock(struct array_queue_lock *l)
{
    unsigned long next = (l->head + 1) % MAX_CPUS;

    l->queue[l->head].flag = 0;
    l->head = next;

    write_release(&l->queue[l->head].flag, 1);
}


/*
 * List-based queueing lock
 *
 * Optimal fair implementation where every CPU gets its own variable to spin on.
 */

struct CACHE_ALIGNED list_queue_node
{
    int flag;
    struct list_queue_node *next;
};

struct CACHE_ALIGNED list_queue_lock
{
    struct list_queue_node *cur;
    struct list_queue_node *tail;
};

void spin_init(struct list_queue_lock *l)
{
    l->cur = nullptr;
    l->tail = nullptr;
}

void spin_lock(struct list_queue_lock *l)
{
    struct list_queue_node node = {0};

    struct list_queue_node *prev = fetch_and_store(&l->tail, &node);
    if (prev) {
        /* This store may theoretically be reordered with the (first, until the branch) load of node.flag below.
         * This is harmless, because until we set the next pointer, potential parallel write-release unlocker
         * is not able to write to the flag variable and unlock us. */
        prev->next = &node;
        while (!atomic_load(&node.flag))
            ;
    }

    acquire_barrier();

    /* This store comes after a read-acquire so technically it is a part of a critical section.
     * Thus the paired load in spin_unlock can only be done later on the same cpu, and thus obeys
     * happens-before relationship with this store. */
    l->cur = &node;
}

void spin_unlock(struct list_queue_lock *l)
{
    struct list_queue_node *cur = l->cur;

    if (!cur->next) {
        if (compare_and_swap(&l->tail, cur, (struct list_queue_node*)NULL) == cur)
            return;

        while (!READ_ONCE(cur->next))
            ;
    }

    write_release(&cur->next->flag, 1);
}


/*
 * Tests
 */

#include <thread>
#include <atomic>
#include <assert.h>

template <typename lock_type>
static void test_spinlock()
{
    constexpr unsigned long num_iters = 1000000;
    unsigned long counter = 0;
    lock_type l;

    spin_init(&l);
    auto func = [&](void) {
        for (int i = 0; i < num_iters; ++i) {
            spin_lock(&l);
            ++counter;
            spin_unlock(&l);
        }
    };

    std::thread workers[MAX_CPUS] = {
        std::thread(func),
        std::thread(func),
        std::thread(func),
        std::thread(func),
    };

    for (auto &t : workers) {
        t.join();
    }

    assert(counter == num_iters * MAX_CPUS);
}

int main(void)
{
    {
        int v = 0;
        assert(0 == test_and_set(&v));
        assert(v == 1);
        assert(1 == test_and_set(&v));
        assert(v == 1);
    }

    {
        int v = 0;
        assert(0 == compare_and_swap(&v, 0, 1));
        assert(v == 1);
        assert(1 == compare_and_swap(&v, 0, 2));
        assert(v == 1);
    }

    {
        int v = 0;
        assert(0 == fetch_and_inc(&v));
        assert(v == 1);
        assert(1 == fetch_and_inc(&v));
        assert(v == 2);
    }

    {
        int v = 0;
        assert(0 == fetch_and_store(&v, 1));
        assert(v == 1);
    }

    test_spinlock<delay_lock>();
    test_spinlock<ticket_lock>();
    test_spinlock<array_queue_lock>();
    test_spinlock<list_queue_lock>();

    return 0;
}
