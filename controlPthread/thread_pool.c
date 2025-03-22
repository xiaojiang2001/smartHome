#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_pool.h"
#include "socket_control.h"


// 创建线程池
ThreadPool* createThreadPool(int size) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    pool->tasks = (ClientTask*)malloc(size * sizeof(ClientTask));
    pool->front = 0;
    pool->rear = 0;
    pool->count = 0;
    pool->size = size;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notEmpty, NULL);
    pthread_cond_init(&pool->notFull, NULL);
    return pool;
}
// 销毁线程池
void destroyThreadPool(ThreadPool* pool) {
    free(pool->tasks);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);
    free(pool);
}
// 线程入队
void enqueue(ThreadPool* pool, int c_fd, struct InputCommand* socketHandler) {
    pthread_mutex_lock(&pool->lock);
    while (pool->count == pool->size) {
        pthread_cond_wait(&pool->notFull, &pool->lock);
    }
    pool->tasks[pool->rear].c_fd = c_fd;
    pool->tasks[pool->rear].socketHandler = socketHandler;
    pool->rear = (pool->rear + 1) % pool->size;
    pool->count++;
    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->lock);
}
// 出队
ClientTask dequeue(ThreadPool* pool) {
    pthread_mutex_lock(&pool->lock);
    while (pool->count == 0) {
        pthread_cond_wait(&pool->notEmpty, &pool->lock);
    }
    ClientTask task = pool->tasks[pool->front];
    pool->front = (pool->front + 1) % pool->size;
    pool->count--;
    pthread_cond_signal(&pool->notFull);
    pthread_mutex_unlock(&pool->lock);
    return task;
}

void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        ClientTask task = dequeue(pool);
        handle_client_message(task.c_fd, task.socketHandler);
    }
    return NULL;
}