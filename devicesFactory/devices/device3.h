#ifndef _DEVICE3_H_
#define _DEVICE3_H_

#include "../devices.h"
#include <stdio.h>
#include <string.h>


// 设备3硬件引脚
#define DEVICE3_PIN_NUM 25
// 设备3触发方式
#define DEVICE3_TRIGGER_MODE HIGH
// 设备3软件名称
#define DEVICE3_NAME "device3"


// 函数指针实际指向的函数，即devide的方法
int device3_init();
int device3_open();
int device3_close();
int device3_readStatue();
void device3_setStatus(int status);

#endif