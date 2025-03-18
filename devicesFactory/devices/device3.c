#include "device3.h"

// 实例化一个设备
struct Device device3 = {
    // 属性
    .deviceName = DEVICE3_NAME,
    .pinNum = DEVICE3_PIN_NUM,
    .status = !DEVICE3_TRIGGER_MODE,
    .triggerMode = DEVICE3_TRIGGER_MODE,

    // 方法
    .init = device3_init,
    .open = device3_open,
    .close = device3_close,
    .readStatus = device3_readStatue,
    .setStatus = device3_setStatus
};

// 将设备1加入设备工厂链表 头插法
struct Device  *addDevice3ToDeviceLink(struct Device *head)
{
    device3_init();
    if(head == NULL)
        return &device3;
    device3.next = head;
    return &device3;
}

// 设备2功能实际的函数
int device3_init()
{
    // 成员属性初始化
    strcpy(device3.deviceName, DEVICE3_NAME);
    device3.pinNum = DEVICE3_PIN_NUM,
    device3.status = !DEVICE3_TRIGGER_MODE,
    device3.triggerMode = DEVICE3_TRIGGER_MODE,

    // 其他初始化
    pinMode(device3.pinNum, OUTPUT);
	digitalWrite(device3.pinNum, !device3.triggerMode);

    printf("%s init success\n", device3.deviceName);
}

int device3_open()
{
    digitalWrite(device3.pinNum, device3.triggerMode);
}

int device3_close()
{
    digitalWrite(device3.pinNum, !device3.triggerMode);
}

int device3_readStatue()
{
    return digitalRead(device3.pinNum);
}

// 成功返回0，失败返回-1
void device3_setStatus(int status)
{
    if(LOW == status)
        digitalWrite(device3.pinNum, LOW);
    else
        digitalWrite(device3.pinNum, HIGH);
}