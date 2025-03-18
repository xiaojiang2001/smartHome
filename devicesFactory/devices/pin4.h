#ifndef _DEVICE3_H_
#define _DEVICE3_H_

#include "devices.h"
#include <stdio.h>
#include "defs.h" 
#include <string.h>


// pin4 触发方式
#define PIN4_TRIGGER_MODE HIGH

// pin4 软件名称
#define PIN4_NAME "pin4"


// 函数指针实际指向的函数，即devide的方法
int pin4_init();
int pin4_open();
int pin4_close();

#endif