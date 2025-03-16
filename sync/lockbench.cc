#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <numa.h>
#include <cstdio>
#include <vector>
#include <atomic>
#include <numeric>
#include <cmath>
#include <chrono>

//#define LOCK_IMPL_SIMPLE
//#define LOCK_IMPL_TICKET
//#define LOCK_IMPL_LIST_QUEUEING

#include "spinlock.cc"

using hrclock = std::chrono::high_resolution_clock;

static constexpr int numIterations = 1000000;
static std::vector<unsigned long long> durations;
static std::atomic<unsigned int> threadsRemaining;
static spinlock_t lock;
static unsigned long long counter;

void* ThreadFunction(void* arg)
{
    int cpuId = sched_getcpu();

    pid_t tid = syscall(SYS_gettid);

    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        exit(-1);
    }

    threadsRemaining--;
    while (threadsRemaining > 0)
        ;

    auto startTime = hrclock::now();
    for (int i = 0; i < numIterations; i++) {
        spin_lock(&lock);
        counter++;
        spin_unlock(&lock);
    }

    auto endTime = hrclock::now();
    unsigned long long duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    printf("Thread %04d / CPU %02d / node %02d: took %llu us\n", tid, cpuId, numa_node_of_cpu(cpuId), duration);
    durations[cpuId] = duration;

    return nullptr;
}

int main()
{
    int numCPUs = sysconf(_SC_NPROCESSORS_ONLN);
    std::vector<pthread_t> threads(numCPUs);
    std::vector<int> cpuIds(numCPUs); // Separate storage for CPU IDs
    pthread_attr_t attr; // Thread attributes

    threadsRemaining = numCPUs;
    durations.resize(numCPUs);

    spin_init(&lock);
    counter = 0;

    int cpuId = sched_getcpu();
    printf("Native cpu %d / node %d\n", cpuId, numa_node_of_cpu(cpuId));

    for (int i = 0; i < numCPUs; i++)
    {
        pthread_attr_init(&attr);

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset) != 0) {
            return EXIT_FAILURE;
        }

        if (pthread_create(&threads[i], &attr, ThreadFunction, nullptr) != 0) {
            return EXIT_FAILURE;
        }

        pthread_attr_destroy(&attr);
    }

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }

    double mean = static_cast<double>(std::accumulate(durations.begin(), durations.end(), 0ULL)) / durations.size();
    double variance = std::accumulate(durations.begin(), durations.end(), 0.0,
                            [mean](double acc, unsigned long long val) {
                                return (static_cast<double>(val) - mean) * (static_cast<double>(val) - mean) + acc;
                            }) / durations.size();
    double pstdev = std::sqrt(variance);

    printf("Mean: %f\nPdev: %f\n", mean, pstdev);
    return 0;
}
