#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#define MAX_QUEUE_SIZE 100  // 队列的最大长度
#define MAX_THREADS 10      // 线程池中线程的最大数量

typedef struct {
    int c_fd;
    struct InputCommand* socketHandler;
} ClientTask;               // 客户端任务结构体

typedef struct {
    ClientTask* tasks;      // 任务队列
    int front;              // 队首指针
    int rear;               // 队尾指针
    int count;              // 当前队列中任务数量
    int size;               // 队列最大长度
    pthread_mutex_t lock;   // 互斥锁
    pthread_cond_t notEmpty;    // 条件变量
    pthread_cond_t notFull;     // 条件变量
} ThreadPool;                   // 线程池结构体

ThreadPool* createThreadPool(int size);         // 创建线程池
void destroyThreadPool(ThreadPool* pool);       // 销毁线程池
void enqueue(ThreadPool* pool, int c_fd, struct InputCommand* socketHandler);   // 入队
ClientTask dequeue(ThreadPool* pool);           // 出队
void* worker(void* arg);

#endif // THREAD_POOL_H