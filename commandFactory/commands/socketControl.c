#include "socketControl.h"
#include "command.h"
#include<wiringPi.h>
#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

struct InputCommand socketContrl = {
	.deviceName = SOCKET_DEVICE_NAME,
	.command = {'\0'},
	.ipAddr = SOCKET_IP_ADDR,
	.port = SOCKET_IP_PORT,
	.log = {'\0'},
	.init = socketInit,
	.getCommand = socketGetCommand,
	.next = NULL
};

int socketInit(struct InputCommand *socketMes)
{
	//1.socket
	int s_fd = socket(AF_INET,SOCK_STREAM, 0);
	if(s_fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	socketMes->fd = s_fd;

	// 设置监听套接字为非阻塞模式
    setnonblocking(s_fd);

	//设置套接字属性 允许地址重用
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);

	//2.bind
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	// s_addr.sin_port = htons(atoi(socketMes->port));
	// inet_aton(socketMes->ipAddr, &(s_addr.sin_addr));
	s_addr.sin_addr.s_addr = inet_addr(socketMes->ipAddr);  
	s_addr.sin_port = htons(atoi(socketMes->port));
	if(bind(s_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        close(s_fd);
        exit(EXIT_FAILURE);
	}
		
	//3.listen
    // 监听
    if (listen(s_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(s_fd);
        exit(EXIT_FAILURE);
    }

   	// 创建epoll实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        close(s_fd);
        exit(EXIT_FAILURE);
    }

	// 将监听套接字添加到epoll实例中
	struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = s_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s_fd, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        close(s_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
	socketMes->epoll_fd = epoll_fd;
	socketMes->fd = s_fd;
	socketMes->getCommand = socketGetCommand;
	
	return s_fd;
}

int socketGetCommand(struct InputCommand *socketMes)
{
	int nread = 0;
	char buf[64] = {'\0'};
	memset(buf, '\0', sizeof(buf));
	memset(socketMes->commandName, '\0', sizeof(socketMes->commandName));
	memset(socketMes->command, '\0', sizeof(socketMes->command));

	nread = read(socketMes->c_fd, buf, sizeof(buf));
	if(nread <= 0)
		return -1;

	printf("get len:%zu, buf:%s\n", strlen(buf), buf);
	if(split_string(buf, socketMes->commandName, socketMes->command, '-') < 0){
		printf("split_string error\n");
		return -1;
	}
	return nread;
}

// 添加设备到工厂链表
struct InputCommand* addSocketContrlToInputCommandLink(struct InputCommand *phead)
{
	if(phead == NULL)
		return &socketContrl;

	socketContrl.next = phead;
	return &socketContrl;
}


// 设置套接字为非阻塞模式
int setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        return -1;
    }
    return 0;
}
