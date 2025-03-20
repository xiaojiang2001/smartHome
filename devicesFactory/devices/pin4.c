#include "pin4.h"
#include <fcntl.h>
#include <unistd.h>

// 实例化一个设备
struct Device pin4 = {
    // 属性
    .deviceName = PIN4_NAME,            // 设备名称 用于在链表中查找的
    .status = !PIN4_TRIGGER_MODE,
    .triggerMode = PIN4_TRIGGER_MODE,

    // 方法
    .init = pin4_init,
    .open = pin4_open,
    .close = pin4_close,
};

// 将pin4加入设备工厂链表 头插法
struct Device  *addPin4ToDeviceLink(struct Device *head)
{
    pin4_init();
    if(head == NULL)
        return &pin4;
    pin4.next = head;
    return &pin4;
}

// pin4功能实际的函数
int pin4_init()
{
    // 成员属性初始化
    strcpy(pin4.deviceName, PIN4_NAME);
    pin4.status = !PIN4_TRIGGER_MODE,
    pin4.triggerMode = PIN4_TRIGGER_MODE,

    // I/O端口初始化 这里调用自己加载的pin4 I/O模块设备
    pin4.fd = open("/dev/pin4",O_RDWR);
    if(pin4.fd  < 0){
        printf("open error\n");
        perror("reason:");
        return -1;
    }
    printf("%s init success\n", pin4.deviceName);
    return 0;
}


int pin4_open()
{
    int cmd = pin4.triggerMode;
    write(pin4.fd, &cmd, 1);
    return 0;
}

int pin4_close()
{
    int cmd = !pin4.triggerMode;
    write(pin4.fd, &cmd, 1);
    return 0;
}