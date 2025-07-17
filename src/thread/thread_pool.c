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
    if (!pool->threads) {
        fprintf(stderr, "thread_pool_init: failed to allocate thread array\n");
        return false;
    }

    pool->thread_count = thread_count;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = false;

    if (pthread_mutex_init(&pool->mutex, NULL) != 0) {
        fprintf(stderr, "thread_pool_init: mutex init failed\n");
        free(pool->threads);
        return false;
    }

    if (pthread_cond_init(&pool->cond, NULL) != 0) {
        fprintf(stderr, "thread_pool_init: cond init failed\n");
        pthread_mutex_destroy(&pool->mutex);
        free(pool->threads);
        return false;
    }

    for (size_t i = 0; i < thread_count; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_main, pool) != 0) {
            fprintf(stderr, "thread_pool_init: failed to create thread\n");
            pool->shutdown = true;
            for (size_t j = 0; j < i; j++)
                pthread_join(pool->threads[j], NULL);
            pthread_cond_destroy(&pool->cond);
            pthread_mutex_destroy(&pool->mutex);
            free(pool->threads);
            return false;
        }
    }

    return true;
}

bool thread_pool_submit(ThreadPool *pool, TaskFunc func, void *arg)
{
    if (!pool || !func)
        return false;

    pthread_mutex_lock(&pool->mutex);
    if (pool->shutdown || pool->count == TASK_QUEUE_SIZE) {
        pthread_mutex_unlock(&pool->mutex);
        return false; /* queue full or shutting down */
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
    if (!pool->threads)
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
