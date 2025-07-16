#ifndef THREAD_THREAD_POOL_H
#define THREAD_THREAD_POOL_H

#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TaskFunc)(void *arg);

typedef struct Task {
    TaskFunc func;
    void *arg;
} Task;

#define TASK_QUEUE_SIZE 32

typedef struct ThreadPool {
    pthread_t *threads;
    size_t thread_count;

    Task queue[TASK_QUEUE_SIZE];
    size_t head;
    size_t tail;
    size_t count;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool shutdown;
} ThreadPool;

bool thread_pool_init(ThreadPool *pool, size_t thread_count);
bool thread_pool_submit(ThreadPool *pool, TaskFunc func, void *arg);
void thread_pool_shutdown(ThreadPool *pool);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_THREAD_POOL_H */
