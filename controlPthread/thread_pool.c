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
    pool->is_running = 1;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notEmpty, NULL);
    pthread_cond_init(&pool->notFull, NULL);
    return pool;
}
// 销毁线程池
void destroyThreadPool(ThreadPool* pool) {
    stopThreadPool(pool);
    clearThreadPool(pool);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);
    free(pool->tasks);
    free(pool);
}
// 线程入队
void enqueue(ThreadPool* pool, int c_fd, struct InputCommand* socketHandler) {
    pthread_mutex_lock(&pool->lock);
    // 如果任务队列已满，等待队列未满的条件变量
    while (pool->count == pool->size) {
        pthread_cond_wait(&pool->notFull, &pool->lock);
    }
    if(pool->is_running) {
        // 将任务添加到队列尾部
        pool->tasks[pool->rear].c_fd = c_fd;                    
        pool->tasks[pool->rear].socketHandler = socketHandler;  
        // 更新队列尾指针
        pool->rear = (pool->rear + 1) % pool->size;
        // 增加任务计数
        pool->count++;
        // 通知有任务加入队列
        pthread_cond_signal(&pool->notEmpty);
    }
    pthread_mutex_unlock(&pool->lock);
}
// 出队
ClientTask dequeue(ThreadPool* pool) {
    pthread_mutex_lock(&pool->lock);
    // 如果任务队列为空，等待队列非空的条件变量
    while (pool->count == 0) {
        pthread_cond_wait(&pool->notEmpty, &pool->lock);
    }
    ClientTask task;
    if (pool->count > 0) {
        task = pool->tasks[pool->front];        // 获取任务
        pool->front = (pool->front + 1) % pool->size;   // 更新队列头指针
        pool->count--;                          // 减少任务计数
        pthread_cond_signal(&pool->notFull);
    } else {
        task.c_fd = -1;  // 表示没有任务
        task.socketHandler = NULL;
    }
    pthread_mutex_unlock(&pool->lock);
    return task;
}

void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (pool->is_running) {
        ClientTask task = dequeue(pool);
        // 处理客户端请求
        if (task.c_fd != -1) {
            handle_client_message(task.c_fd, task.socketHandler);
        }
    }
    return NULL;
}

// 停止线程池
void stopThreadPool(ThreadPool* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->is_running = 0;
    pthread_cond_broadcast(&pool->notEmpty);  // 唤醒所有等待的线程
    pthread_cond_broadcast(&pool->notFull);   // 唤醒所有等待的线程
    pthread_mutex_unlock(&pool->lock);
    printf("Thread pool stopped.\n");
}

// 清空线程池
void clearThreadPool(ThreadPool* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->front = 0;
    pool->rear = 0;
    pool->count = 0;
    pthread_mutex_unlock(&pool->lock);
    printf("Thread pool cleared.\n");
}