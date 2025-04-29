#include "device1.h"

// 实例化一个设备
struct Device device1 = {
    // 属性
    .deviceName = DEVICE1_NAME,
    .pinNum = DEVICE1_PIN_NUM,
    .status = !DEVICE1_TRIGGER_MODE,
    .triggerMode = DEVICE1_TRIGGER_MODE,

    // 方法
    .init = device1_init,
    .open = device1_open,
    .close = device1_close,
    .readStatus = device1_readStatue,
    .setStatus = device1_setStatus
};

// 将设备1加入设备工厂链表 头插法
struct Device  *addDevice1ToDeviceLink(struct Device *head)
{
    device1_init();
    if(head == NULL)
        return &device1;
    device1.next = head;
    return &device1;
}

// 设备1功能实际的函数
int device1_init()
{
    // 成员属性初始化
    strcpy(device1.deviceName, DEVICE1_NAME);
    device1.pinNum = DEVICE1_PIN_NUM;
    device1.status = !DEVICE1_TRIGGER_MODE;
    device1.triggerMode = DEVICE1_TRIGGER_MODE;

    // 初始化互斥锁
    if (pthread_mutex_init(&device1.ioLock, NULL) != 0) {
        perror("Mutex init failed");
        return -1;
    }

    // 其他初始化
    pthread_mutex_lock(&device1.ioLock);
    pinMode(device1.pinNum, OUTPUT);
	digitalWrite(device1.pinNum, !device1.triggerMode);
    pthread_mutex_unlock(&device1.ioLock);
    
    printf("%s init success\n", device1.deviceName);

    return 0;
}

int device1_open()
{
    pthread_mutex_lock(&device1.ioLock);
    digitalWrite(device1.pinNum, device1.triggerMode);
    pthread_mutex_unlock(&device1.ioLock);
    return 0;
}

int device1_close()
{
    pthread_mutex_lock(&device1.ioLock);
    digitalWrite(device1.pinNum, !device1.triggerMode);
    pthread_mutex_unlock(&device1.ioLock);
    return 0;
}

int device1_readStatue()
{
    pthread_mutex_lock(&device1.ioLock);
    int status = digitalRead(device1.pinNum);
    pthread_mutex_unlock(&device1.ioLock);
    return status;
}

// 成功返回0，失败返回-1
void device1_setStatus(int status)
{
    pthread_mutex_lock(&device1.ioLock);
    if(LOW == status)
        digitalWrite(device1.pinNum, LOW);
    else
        digitalWrite(device1.pinNum, HIGH);
    pthread_mutex_unlock(&device1.ioLock);
}