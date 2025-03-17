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

// // 服务器初始化
// int socketInit(struct InputCommand *socketMes);
// // 获取指令
// int socketGetCommand(struct InputCommand *socketMes);

struct InputCommand socketContrl = {
	.commandName = COMMAND_NAME,
	.command = '\0',
	.ipAddr = SOCKET_IP_ADDR,
	.port = SOCKET_IP_PORT,
	.log = '\0',
	.init = socketInit,
	.getCommand = socketGetCommand,
	.next=NULL
};

int socketInit(struct InputCommand *socketMes)
{
	//1.socket
	int s_fd = socket(AF_INET,SOCK_STREAM, 0);
	if(s_fd == -1) {
		perror("socket");
		return -1;
	}


	 //设置套接字属性 允许地址重用
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);

	//2.bind
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(socketMes->port));
	inet_aton(socketMes->ipAddr, &(s_addr.sin_addr));
	if(bind(s_fd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		return -1;
	}
		

	//3.listen
	if(listen(s_fd, 10) < 0) {
		perror("bind");
		return -1;
	}

	printf("socket Server listening ...\n");
	socketMes->fd = s_fd;

	return s_fd;
}


int socketGetCommand(struct InputCommand *socketMes)
{
	int nread = 0;
	char buf[128] = {'\0'};
	memset(buf, '\0', sizeof(buf));
	memset(socketMes->commandName, '\0', sizeof(socketMes->commandName));
	memset(socketMes->command, '\0', sizeof(socketMes->command));


	nread = read(socketMes->c_fd, buf, sizeof(buf));
	if(nread <= 0)
		return -1;
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

