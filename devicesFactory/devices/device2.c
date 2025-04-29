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
    device2.pinNum = DEVICE2_PIN_NUM;
    device2.status = !DEVICE2_TRIGGER_MODE;
    device2.triggerMode = DEVICE2_TRIGGER_MODE;

    // 初始化互斥锁
    if (pthread_mutex_init(&device2.ioLock, NULL) != 0) {
        perror("Mutex init failed");
        return -1;
    }

    // 其他初始化
    pthread_mutex_lock(&device2.ioLock);
    pinMode(device2.pinNum, OUTPUT);
	digitalWrite(device2.pinNum, !device2.triggerMode);
    pthread_mutex_unlock(&device2.ioLock);  
    printf("%s init success\n", device2.deviceName);
    return 0;
}

int device2_open()
{
    pthread_mutex_lock(&device2.ioLock);
    digitalWrite(device2.pinNum, device2.triggerMode);
    pthread_mutex_unlock(&device2.ioLock);  
    return 0;
}

int device2_close()
{
    pthread_mutex_lock(&device2.ioLock);
    digitalWrite(device2.pinNum, !device2.triggerMode);
    pthread_mutex_unlock(&device2.ioLock);  
    return 0;
}

int device2_readStatue()
{
    pthread_mutex_lock(&device2.ioLock);
    int status = digitalRead(device2.pinNum);
    pthread_mutex_unlock(&device2.ioLock);  
    return status;
}

// 成功返回0，失败返回-1
void device2_setStatus(int status)
{
    pthread_mutex_lock(&device2.ioLock);
    if(LOW == status)
        digitalWrite(device2.pinNum, LOW);
    else
        digitalWrite(device2.pinNum, HIGH);
    pthread_mutex_unlock(&device2.ioLock);  
}