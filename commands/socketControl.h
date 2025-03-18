#ifndef _SOCKETCONTROL_H_
#define _SOCKETCONTROL_H_

// 自定义设备IP地址与端口号
#define SOCKET_IP_ADDR 	"192.168.10.203"
#define SOCKET_IP_PORT 	"8082"


// 定义一个服务器名称 随便什么即可
#define SOCKET_DEVICE_NAME "socketServer"

// 前向声明，而不是具体定义  
struct InputCommand;  
// 服务器初始化
int socketInit(struct InputCommand *socketMes);
// 获取指令
int socketGetCommand(struct InputCommand *socketMes);


#endif