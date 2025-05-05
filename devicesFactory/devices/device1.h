#ifndef _DEVICE1_H_
#define _DEVICE1_H_

#include "../devices.h"
#include <stdio.h>
#include <string.h>


// 设备1硬件引脚
#define DEVICE1_PIN_NUM 0
// 设备1触发方式
#define DEVICE1_TRIGGER_MODE HIGH
// 设备1软件名称
#define DEVICE1_NAME "device1"



// 函数指针实际指向的函数，即devide的方法
int device1_init();
int device1_open();
int device1_close();
int device1_readStatue();
void device1_setStatus(int status);




#endif