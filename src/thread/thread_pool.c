#include "thread/thread_pool.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static void *worker_main(void *arg)
{
    ThreadPool *pool = (ThreadPool*)arg;
    for (;;) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->count == 0 && !pool->shutdown)
            pthread_cond_wait(&pool->cond, &pool->mutex);
        if (pool->shutdown && pool->count == 0) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }
        Task task = pool->queue[pool->head];
        pool->head = (pool->head + 1) % TASK_QUEUE_SIZE;
        pool->count--;
        pthread_mutex_unlock(&pool->mutex);
        task.func(task.arg);
    }
    return NULL;
}

bool thread_pool_init(ThreadPool *pool, size_t thread_count)
{
    if (!pool || thread_count == 0 || thread_count > TASK_QUEUE_SIZE)
        return false;
    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    if (!pool->threads)
        return false;
    pool->thread_count = thread_count;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = false;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    for (size_t i = 0; i < thread_count; i++)
        pthread_create(&pool->threads[i], NULL, worker_main, pool);
    return true;
}

bool thread_pool_submit(ThreadPool *pool, TaskFunc func, void *arg)
{
    if (!pool || !func)
        return false;
    pthread_mutex_lock(&pool->mutex);
    if (pool->count == TASK_QUEUE_SIZE) {
        pthread_mutex_unlock(&pool->mutex);
        return false; /* queue full */
    }
    pool->queue[pool->tail].func = func;
    pool->queue[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % TASK_QUEUE_SIZE;
    pool->count++;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    return true;
}

void thread_pool_shutdown(ThreadPool *pool)
{
    if (!pool)
        return;
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    for (size_t i = 0; i < pool->thread_count; i++)
        pthread_join(pool->threads[i], NULL);
    free(pool->threads);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}
