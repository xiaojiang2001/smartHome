#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringSerial.h>

#include "devices.h"    // 设备工厂
#include "command.h"    // 指令工厂
#include "device1.h"    // 1号设备
#include "device2.h"    // 2号设备
#include "device3.h"    // 3号设备
#include "voiceControl.h"       // 指令来源1
#include "socketControl.h"      // 指令来源2


struct Device* pdeviceHead = NULL;
struct InputCommand* pCommandHead = NULL;


// 负责接收客户端消息
void *read_thread(void* datas)
{
    int nread;			// 读取到的字节数
    struct InputCommand* socketHandler;
    socketHandler  = (struct InputCommand*)datas;
    struct Device* device = NULL;

    char send_buf[128] = "test send data";
    write(socketHandler->c_fd, send_buf, strlen(send_buf));  //测试发送数据
    while(1)
    {
		//接收，返回接收到的字节数放入n_read
        nread = socketHandler->getCommand(socketHandler);
        if(nread <= 0)
            continue;

        //通过名称找设备，设备名称存device
        device = findDeviceByNanme(pdeviceHead, socketHandler->commandName);
        if(device != NULL) 
        {
            printf("len: %d, command:%s\n", strlen(socketHandler->command), socketHandler->command);


            if(strcmp(socketHandler->command, "open") == 0)
                device->open();
            else if(strcmp(socketHandler->command, "close") == 0)
                device->close();
            else
                printf("command not exist\n");
        }
        else
            printf("device not exist\n");
    }     
    pthread_exit(NULL);
}


// 接收客户端连接线程
void* socket_thread(void* data)
{
    struct InputCommand *socketHandler;
    int nread = 0;
    struct sockaddr c_addr;
    int clen = sizeof(struct sockaddr_in);
    memset(&c_addr, 0 , clen);
    
    // 通过指令名称找指令句柄
    socketHandler = findCommandByNanme(pCommandHead, SOCKET_DEVICE_NAME);
    if(socketHandler == NULL){
        printf("socketHandler is null\n");
        pthread_exit(NULL);
    }

    if(socketHandler -> init(socketHandler) < 0) {
        printf("socket_thread: %s init error\n", socketHandler->commandName);
        pthread_exit(NULL);
    }
    printf("%s init success\n", socketHandler->deviceName);

    pthread_t readThread;

    while (1)
    {
        // 收到一个客户端连接，创建一个客户端线程
        int c_fd = accept(socketHandler->fd, (struct sockaddr *)&c_addr, &clen);
        socketHandler->c_fd = c_fd;
        printf("new client connect: c_fd = %d\n", c_fd);
        pthread_create(&readThread, NULL, read_thread, (void *)socketHandler);
    }
}

void* voice_thread(void* data)
{
    int nread = 0;

    // 指令工厂中的指令  找到对应的指令类型
    struct InputCommand *voiceHandler = findCommandByNanme(pCommandHead, VOICER_DEVICE_NAME); 
    if(voiceHandler == NULL){
        printf("voiceHandler is null\n");
        return NULL;
    }
    // 初始化指令
    if(voiceHandler -> init(voiceHandler) < 0) {
        printf("voice_thread: voice_init error\n");
        pthread_exit(NULL);
    }
    printf("%s init success\n", voiceHandler->deviceName);

    //测试发送数据
    char send_buf[128] = "test send data";
    write(voiceHandler->fd, send_buf, strlen(send_buf));  

    struct Device *device = NULL;               // 设备工厂中的设备
    while (1)
    {
        while(serialDataAvail(voiceHandler->fd) != -1 ) //串口有数据
        {
            nread = voiceHandler->getCommand(voiceHandler);
            if(nread <= 0)
                continue;

            //通过名称找设备，设备名称存入tmp
            device = findDeviceByNanme(pdeviceHead, voiceHandler->commandName);
            if(device != NULL) {
                device->init();
                if(strcmp(voiceHandler->command, "open") == 0)
                    device->open();
                else if(strcmp(voiceHandler->command, "close") == 0)
                    device->close();
                else
                    printf("command not exist\n");
            }
            else
                printf("device not exist\n");
        }
    }
}

int main()
{
    char *deviceName = "device2";

    // 初始化wiringPi库
    if(-1 == wiringPiSetup()) {
        printf("wiringPiSetup error\n");
        return ERROR;
    }
 
    // 设备工程初始化
    pdeviceHead = addDevice1ToDeviceLink(pdeviceHead);      // 将设备1加入设备工厂 链式存储 头插法
    pdeviceHead = addDevice2ToDeviceLink(pdeviceHead);      // 将设备2加入设备工厂 链式存储 头插法
    pdeviceHead = addDevice3ToDeviceLink(pdeviceHead);      // 将设备3加入设备工厂 链式存储 头插法
    pdeviceHead = addPin4ToDeviceLink(pdeviceHead);         // 将I/o驱动加入设备工厂 上层调用
    // 指令工厂初始化
    pCommandHead = addSocketContrlToInputCommandLink(pCommandHead); //指令控制1加入指令工厂 链式存储 头插法
    pCommandHead = addVoiceContrlToInputCommandLink(pCommandHead);  //指令控制2加入指令工厂 链式存储 头插法
    

    //进程变量
    pthread_t socketThread;
	pthread_t voiceThread;
    pthread_create(&socketThread, NULL, socket_thread, NULL);
    pthread_create(&voiceThread,  NULL, voice_thread,  NULL);

    pthread_join(socketThread, NULL);
    pthread_join(voiceThread, NULL);
    return 0;
    // 线程池创建
}