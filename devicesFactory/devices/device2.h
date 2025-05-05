#ifndef _DEVICE2_H_
#define _DEVICE2_H_

#include "../devices.h"
#include <stdio.h>
#include <string.h>

// 设备2硬件引脚
#define DEVICE2_PIN_NUM         1
// 设备2触发方式
#define DEVICE2_TRIGGER_MODE    HIGH
// 设备2软件名称
#define DEVICE2_NAME            "device2"


// 函数指针实际指向的函数，即devide的方法
int device2_init();
int device2_open();
int device2_close();
int device2_readStatue();
void device2_setStatus(int status);




#endif