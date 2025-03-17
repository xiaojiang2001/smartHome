#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "wiringPi.h"

// 设备工厂

// 定义每个设备
struct Device
{
    char deviceName[128];       // 设备名称
    int status;                 // 开关状态
    int pinNum;                 // 硬件引脚号
    int triggerMode;            // 触发方式

    int (*init)();              // 初始化设备
    int (*open)();              // 开灯
    int (*close)();             // 关灯
    int (*readStatus)();        // 获取设备状态
    void (*setStatus)(int status);    // 设置状态
    
    struct Device* next;        // next指针
};

// 在主函数调用, 设备插入设备工厂
struct Device *addDevice1ToDeviceLink(struct Device *head);
struct Device *addDevice2ToDeviceLink(struct Device *head);
struct Device *addDevice3ToDeviceLink(struct Device *head);

// 指令插入指令工厂

// 在设备链表中通过设备名字找设备
struct Device* findDeviceByNanme(struct Device* pdeviceHead, char *deviceName);
#endif