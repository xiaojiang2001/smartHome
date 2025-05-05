#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>  
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>

#include "devices.h"        // 设备工厂
#include "command.h"        // 指令工厂
#include "socketControl.h"  // 指令来源1
#include "thread_pool.h"
#include "common.h"

extern volatile sig_atomic_t is_running;  // 控制线程池运行状态
void handle_client_message(int c_fd, struct InputCommand* socketHandler) {
    int nread;
    char buf[64] = {'\0'};
    memset(buf, '\0', sizeof(buf));
    memset(socketHandler->commandName, '\0', sizeof(socketHandler->commandName));
    memset(socketHandler->command, '\0', sizeof(socketHandler->command));

    nread = read(c_fd, buf, sizeof(buf));
    if (nread <= 0) {
        // 连接关闭
        printf("closed connection on descriptor %d\n", c_fd);
        close(c_fd);
        struct epoll_event ev;
        ev.data.fd = c_fd;
        epoll_ctl(socketHandler->epoll_fd, EPOLL_CTL_DEL, c_fd, &ev);
        return;
    }

    printf("get len:%zu, buf:%s\n", strlen(buf), buf);
     // 给客户端发送测试  
    if(write(c_fd, buf, strlen(buf)) < 0) {
        printf("write error\n");
    }       
    
    if (split_string(buf, socketHandler->commandName, socketHandler->command, '-') < 0) {
        printf("split_string error\n");
        return;
    }

    struct Device* device = findDeviceByNanme(pdeviceHead, socketHandler->commandName);
    if (device != NULL) {
        printf("len: %zu, command:%s\n", strlen(socketHandler->command), socketHandler->command);
        if (strcmp(socketHandler->command, "open") == 0)
            device->open();
        else if (strcmp(socketHandler->command, "close") == 0)
            device->close();
        else if (strcmp(socketHandler->command, "face") == 0) {
            pthread_mutex_lock(&faceMutex);
            while (faceCnt == MAX_QUEUE_SIZE) {
                pthread_cond_wait(&faceNotFullMutex, &faceMutex);
            }
            faceCnt++;
            // 如果队列之前是空的
            if (faceCnt == 1) {
                pthread_cond_signal(&faceNotEmptyMutex);
            }
            pthread_mutex_unlock(&faceMutex);
        } else
            printf("command not exist\n");
    } else
        printf("device not exist\n");
}

void* socket_thread(void* data)
{
    struct InputCommand* pCommandHead = (struct InputCommand *)data;
    // 获取socketHandler
    struct InputCommand *socketHandler = findCommandByNanme(pCommandHead, SOCKET_DEVICE_NAME);
    if (socketHandler == NULL) {
        printf("socketHandler is null\n");
        pthread_exit(NULL);
    }

    // 初始化socketHandler
    if (socketHandler->init(socketHandler) < 0) {
        printf("socket_thread: %s init error\n", socketHandler->commandName);
        pthread_exit(NULL);
    }
    printf("%s init success\n", socketHandler->deviceName);

    // 创建epoll事件数组
    struct epoll_event events[MAX_EVENTS];
    
    // 创建线程池
    ThreadPool* pool = createThreadPool(MAX_THREADS);
    pthread_t workers[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&workers[i], NULL, worker, pool);
    }
    
    while (is_running) 
    {
        // 等待事件发生
        int nfds = epoll_wait(socketHandler->epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        // 处理事件
        for (int i = 0; i < nfds; i++) 
        {
            // 处理监听套接字事件
            if (events[i].data.fd == socketHandler->fd) 
            {
                // 处理新的连接请求
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int conn_fd = accept(socketHandler->fd, (struct sockaddr*)&client_addr, &client_len);
                if (conn_fd == -1) {
                    perror("accept");
                    continue;
                }
                // 打印新的连接信息
                char client_ip[INET_ADDRSTRLEN];
                if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN) != NULL) {
                    printf("New connection from %s:%d on socket %d\n", client_ip, ntohs(client_addr.sin_port), conn_fd);
                }
                else {
                    perror("inet_ntop");
                }

                // 设置新连接的套接字为非阻塞模式
                setnonblocking(conn_fd);

                // 将新连接的套接字添加到epoll实例中
                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if (epoll_ctl(socketHandler->epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    close(conn_fd);
                    continue;
                }
            } 
            // 处理已连接的套接字上的数据
            else if (events[i].events & EPOLLIN) {
                int conn_fd = events[i].data.fd;        // 获取已连接的套接字
                enqueue(pool, conn_fd, socketHandler);  // 添加任务
            }
        }
    }

    // 停止线程池
    stopThreadPool(pool);

    // 清空任务队列
    clearThreadPool(pool);

   // 等待所有工作线程退出
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    // 销毁线程池
    destroyThreadPool(pool);

    close(socketHandler->fd);
    close(socketHandler->epoll_fd);
    pthread_exit(NULL);
}