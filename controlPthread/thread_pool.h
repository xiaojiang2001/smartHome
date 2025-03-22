#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#define MAX_QUEUE_SIZE 100
#define MAX_THREADS 10      // 线程池中线程的最大数量

typedef struct {
    int c_fd;
    struct InputCommand* socketHandler;
} ClientTask;

typedef struct {
    ClientTask* tasks;
    int front;
    int rear;
    int count;
    int size;
    pthread_mutex_t lock;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
} ThreadPool;

ThreadPool* createThreadPool(int size);
void destroyThreadPool(ThreadPool* pool);
void enqueue(ThreadPool* pool, int c_fd, struct InputCommand* socketHandler);
ClientTask dequeue(ThreadPool* pool);
void* worker(void* arg);

#endif // THREAD_POOL_H