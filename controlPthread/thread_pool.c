#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_pool.h"
#include "socket_control.h"

// 创建线程池
ThreadPool* createThreadPool(int size) 
{
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));     // 创建线程池结构体
    pool->tasks = (ClientTask*)malloc(size * sizeof(ClientTask));   //  创建任务队列
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
    // 如果任务队列已满，等待队列未满的条件变量
    while (pool->count == pool->size) {
        pthread_cond_wait(&pool->notFull, &pool->lock);
    }
    // 将任务添加到队列尾部
    pool->tasks[pool->rear].c_fd = c_fd;                    
    pool->tasks[pool->rear].socketHandler = socketHandler;  
    // 更新队列尾指针
    pool->rear = (pool->rear + 1) % pool->size;
    // 增加任务计数
    pool->count++;
    // 通知有任务加入队列
    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->lock);
}
// 出队
ClientTask dequeue(ThreadPool* pool) {
    pthread_mutex_lock(&pool->lock);
    // 如果任务队列为空，等待队列非空的条件变量
    while (pool->count == 0) {
        pthread_cond_wait(&pool->notEmpty, &pool->lock);
    }
    // 从队列头部取出一个任务
    ClientTask task = pool->tasks[pool->front];
    // 更新队列头指针
    pool->front = (pool->front + 1) % pool->size;
    // 减少任务计数
    pool->count--;
    // 通知队列不满
    pthread_cond_signal(&pool->notFull);
    pthread_mutex_unlock(&pool->lock);
    return task;
}

void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        ClientTask task = dequeue(pool);
        // 处理客户端请求
        handle_client_message(task.c_fd, task.socketHandler);
    }
    return NULL;
}