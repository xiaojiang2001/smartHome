#include "devices.h"
#include "my_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


// 创建设备
struct Device* createDevice(DeviceType type, const char* name, int pin)
{
    // 参数检查
    if (name == NULL || strlen(name) >= 128) {
        printf("设备名称无效\n");
        return NULL;
    }

    // 使用堆管理器分配内存
    struct Device* dev = (struct Device*)my_heap.malloc(sizeof(struct Device));
    if (dev == NULL) {
        printf("内存分配失败，剩余堆空间：%u字节\n", my_heap.get_free_heapsize());
        return NULL;
    }

    // 初始化设备基本属性
    memset(dev, 0, sizeof(struct Device));
    dev->type = type;
    dev->pinNum = pin;
    dev->status = DEVICE_STATUS_UNKNOWN;
    strncpy(dev->deviceName, name, sizeof(dev->deviceName) - 1);
    dev->fd = -1;
    dev->triggerMode = 0;
    dev->next = NULL;

    // 初始化互斥锁
    if (pthread_mutex_init(&dev->ioLock, NULL) != 0) {
        printf("互斥锁初始化失败\n");
        free(dev);
        return NULL;
    }

    // 初始化条件变量
    if (pthread_cond_init(&dev->statusCond, NULL) != 0) {
        printf("条件变量初始化失败\n");
        pthread_mutex_destroy(&dev->ioLock);
        free(dev);
        return NULL;
    }

    // 初始化错误处理
    dev->errorCode = 0;
    memset(dev->errorMsg, 0, sizeof(dev->errorMsg));

    return dev;
}

// 销毁设备
void destroyDevice(struct Device* dev)
{
    if (dev == NULL) {
        return;
    }

    // 互斥访问
    pthread_mutex_lock(&dev->ioLock);

    // 如果设备还在运行，先关闭它
    if (dev->status == DEVICE_STATUS_ON && dev->close != NULL) {
        dev->close(dev);
    }

    // 如果注册了deinit函数，调用它清理资源
    if (dev->deinit != NULL) {
        dev->deinit(dev);
    }

    // 清理同步原语
    pthread_cond_destroy(&dev->statusCond);
    pthread_mutex_unlock(&dev->ioLock);
    pthread_mutex_destroy(&dev->ioLock);

    // 释放内存
    free(dev);
}

// 注册设备回调
int registerDeviceCallback(struct Device* dev, DeviceEventCallback callback, void* userData)
{
    if (dev == NULL || callback == NULL) {
        return -1;
    }

    pthread_mutex_lock(&dev->ioLock);
    
    // 设置回调函数和用户数据
    dev->eventCallback = callback;
    dev->userData = userData;
    
    pthread_mutex_unlock(&dev->ioLock);
    return 0;
}

// 通过设备名字找设备
struct Device* findDeviceByName(struct Device* pdeviceHead, const char* deviceName)
{
    struct Device* temp = pdeviceHead;
    if(NULL == pdeviceHead){
        printf("pdeviceHead is null!!!\n");
        return NULL;
    }
    while (temp != NULL)
    {
        // 找到名字一样的设备了
        if(strcmp(temp->deviceName, deviceName) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    // 没有这个名字的设备
    return NULL;
}

// 辅助函数：触发设备事件
void triggerDeviceEvent(struct Device* dev, DeviceEvent event, void* data)
{
    if (dev == NULL || dev->eventCallback == NULL) {
        return;
    }

    pthread_mutex_lock(&dev->ioLock);
    dev->eventCallback(dev, event, data);
    pthread_mutex_unlock(&dev->ioLock);
}



