#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "thread_pool.h"
#include "socket_control.h"
#include "voice_control.h"
#include "face_control.h"
#include "common.h"
#include <signal.h>

volatile sig_atomic_t is_running = 1;  // 控制线程池运行状态

void signal_handler(int signum) {
    if (signum == SIGINT) {
        is_running = 0;  // 设置标志为 0，通知线程池停止
        printf("Received SIGINT, stopping thread pool...\n");
    }
}


int main()
{
    // 初始化wiringPi库
    if(-1 == wiringPiSetup()) {
        printf("wiringPiSetup error\n");
        return -1;
    }

    // 注册信号处理函数
    signal(SIGINT, signal_handler);

    // 设备工程初始化
    pdeviceHead = addDevice1ToDeviceLink(pdeviceHead);      // 将设备1加入设备工厂 链式存储 头插法
    pdeviceHead = addDevice2ToDeviceLink(pdeviceHead);      // 将设备2加入设备工厂 链式存储 头插法
    pdeviceHead = addDevice3ToDeviceLink(pdeviceHead);      // 将设备3加入设备工厂 链式存储 头插法
    
    // 指令工厂初始化
    struct InputCommand* pCommandHead = NULL;
    pCommandHead = addSocketContrlToInputCommandLink(pCommandHead); //指令控制1加入指令工厂 链式存储 头插法
    // pCommandHead = addVoiceContrlToInputCommandLink(pCommandHead);  //指令控制2加入指令工厂 链式存储 头插法
    pCommandHead = addFaceContrlToInputCommandLink(pCommandHead);   

    // 进程变量
    pthread_t socketThread;
    pthread_t voiceThread;
    pthread_t faceThread;
    pthread_create(&socketThread, NULL, socket_thread, (void *)pCommandHead);
    pthread_create(&voiceThread,  NULL, voice_thread,  (void *)pCommandHead);
    pthread_create(&faceThread,   NULL, face_thread,   (void *)pCommandHead);

    pthread_join(socketThread, NULL);
    pthread_join(faceThread, NULL);
    pthread_join(voiceThread, NULL);
    return 0;
}