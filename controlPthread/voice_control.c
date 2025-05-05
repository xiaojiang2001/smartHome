#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringSerial.h>
#include "command.h"    // 指令工厂
#include "common.h"
#include "voiceControl.h"       // 指令来源2
#include <signal.h>


extern volatile sig_atomic_t is_running;  // 控制线程池运行状态

void* voice_thread(void* data)
{
    struct InputCommand* pCommandHead = (struct InputCommand *)data;

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
    while (is_running)
    {
        while(serialDataAvail(voiceHandler->fd) != -1 ) // 串口有数据
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