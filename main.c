#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#include "devices.h"    // 设备工厂
#include "command.h"    // 指令工厂
#include "device1.h"    // 1号设备
#include "device2.h"    // 2号设备
#include "device3.h"    // 3号设备

#include "voiceControl.h"   // 指令来源1
#include "socketControl.h"   // 指令来源2


struct Device* pdeviceHead = NULL;
struct InputCommand* pCommandHead = NULL;

void *read_thread(void* datas)
{
    while (1)
    {
        /* code */
    }
    
}


// 接收客户端连接线程
void* socket_thread(void* data)
{
    struct InputCommand *socketHandler;
    int nread = 0;
    struct sockaddr c_addr;
    int clen = sizeof(struct sockaddr_in);
    memset(&c_addr, 0 , clen);
    
    socketHandler = findCommandByNanme(pCommandHead, "socketServer");
    if(socketHandler == NULL){
        printf("socketHandler is null\n");
        pthread_exit(NULL);
    }

    if(socketHandler -> init(socketHandler) < 0) {
        printf("socket_thread: %s init error\n", socketHandler->commandName);
        pthread_exit(NULL);
    }
    printf("%s init success\n", socketHandler->commandName);

    pthread_t readThread;

    while (1)
    {
        // 收到一个客户端连接，创建线程
        int c_fd = accept(socketHandler->fd, (struct sockaddr *)&c_addr, &clen);
        pthread_create(&readThread, NULL, read_thread, NULL);
    }
}

void* voice_thread(void* data)
{
    struct InputCommand *voiceHandler;
    int nread = 0;
    // 找到对应的指令类型
    voiceHandler = findCommandByNanme(pCommandHead, "voice");
    if(voiceHandler == NULL){
        printf("voiceHandler is null\n");
        return NULL;
    }

    if(voiceHandler -> init(voiceHandler) < 0) {
        printf("voice_thread: voice_init error\n");
        pthread_exit(NULL);
    }
    printf("%s init success\n", voiceHandler->commandName);

    while (1)
    {
       nread = voiceHandler->getCommand(voiceHandler); 
       if(nread > 0)
            printf("get commandtext from voice: %s\n", voiceHandler->command);
        else
            printf("no voice data! ! !\n");
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
    // 指令工厂初始化
    
    pCommandHead = addSocketContrlToInputCommandLink(pCommandHead); //指令控制1加入指令工厂 链式存储 头插法
    pCommandHead = addVoiceContrlToInputCommandLink(pCommandHead);  //指令控制2加入指令工厂 链式存储 头插法
    

    //进程变量
    pthread_t socketThread;
	pthread_t voiceThread;
    pthread_create(&socketThread, NULL, socket_thread, NULL);
    pthread_create(&voiceThread,  NULL, voice_thread,  NULL);

    // 去设备工厂找设备，返回设备节点
    struct Device* device = findDeviceByNanme(pdeviceHead, deviceName);
    if(device == NULL){
        printf("not find\n");
        return 0;
    }

    int cmd = 0;
    sleep(1);

    while (1)
    {
        printf("please input:");
        scanf("%d",&cmd);
        if(1 == cmd)
            device->open();
        else if(0 == cmd)
            device->close();
    }
    
    pthread_join(socketThread, NULL);
    pthread_join(voiceThread, NULL);
    return 0;
    // 线程池创建
}