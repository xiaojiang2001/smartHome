#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"     
#include "command.h"            // 指令工厂
#include "face.h"               // 指令来源3
#include "socketControl.h"
#include "thread_pool.h"
#include "signal.h"


extern volatile sig_atomic_t is_running;  // 控制线程池运行状态

void* face_thread(void* data)
{
    struct InputCommand* pCommandHead = (struct InputCommand *)data;

    char buf[128];
    char img[64] = "commandFactory/commands/img.jpg";

    sprintf(buf, "wget http://%s:8080/?action=snapshot -O %s", SOCKET_IP_ADDR, img);

    // 指令工厂中的指令  找到对应的指令类型
    struct InputCommand *faceHandler = findCommandByNanme(pCommandHead, FACE_DEVICE_NAME); 
    if(faceHandler == NULL){
        printf("faceHandler is null\n");
        return NULL;
    }
    faceHandler->init(faceHandler);

	while(is_running)
	{
        // 等待识别指令
        pthread_mutex_lock(&faceMutex);
        while (faceCnt == 0) {
            pthread_cond_wait(&faceNotEmptyMutex, &faceMutex);
        }
        faceCnt--;
         // 如果队列之前是满的
        if (faceCnt == MAX_QUEUE_SIZE - 1) { 
            pthread_cond_signal(&faceNotFullMutex);
        }
        pthread_mutex_unlock(&faceMutex);

		printf("photeing.......\n");
		system(buf);
        // 保存图片路径到结构体
        strcpy(faceHandler->command, "img.jpg");
        int ret = faceHandler->getCommand(faceHandler);   // 获取识别结果
        if(ret == 0)
            printf("recognize error\n");
        else
            printf("recognize the same person");
	}   
}