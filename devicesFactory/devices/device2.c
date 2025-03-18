#include "device2.h"

// 实例化一个设备
struct Device device2 = {
    // 属性
    .deviceName = DEVICE2_NAME,
    .pinNum = DEVICE2_PIN_NUM,
    .status = !DEVICE2_TRIGGER_MODE,
    .triggerMode = DEVICE2_TRIGGER_MODE,

    // 方法
    .init = device2_init,
    .open = device2_open,
    .close = device2_close,
    .readStatus = device2_readStatue,
    .setStatus = device2_setStatus
};

// 将设备1加入设备工厂链表 头插法
struct Device  *addDevice2ToDeviceLink(struct Device *head)
{
    device2_init();
    if(head == NULL)
        return &device2;
    device2.next = head;
    return &device2;
}

// 设备2功能实际的函数
int device2_init()
{
    // 成员属性初始化
    strcpy(device2.deviceName, DEVICE2_NAME);
    device2.pinNum = DEVICE2_PIN_NUM,
    device2.status = !DEVICE2_TRIGGER_MODE,
    device2.triggerMode = DEVICE2_TRIGGER_MODE,

    // 其他初始化
    pinMode(device2.pinNum, OUTPUT);
	digitalWrite(device2.pinNum, !device2.triggerMode);

    printf("%s init success\n", device2.deviceName);
}

int device2_open()
{
    digitalWrite(device2.pinNum, device2.triggerMode);
}

int device2_close()
{
    digitalWrite(device2.pinNum, !device2.triggerMode);
}

int device2_readStatue()
{
    return digitalRead(device2.pinNum);
}

// 成功返回0，失败返回-1
void device2_setStatus(int status)
{
    if(LOW == status)
        digitalWrite(device2.pinNum, LOW);
    else
        digitalWrite(device2.pinNum, HIGH);
}