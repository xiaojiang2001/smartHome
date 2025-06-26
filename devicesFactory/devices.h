#ifndef _DEVICE_H_
#define _DEVICE_H_
#include <wiringPi.h>
#include <pthread.h>

// 设备类型枚举
typedef enum {
    DEVICE_TYPE_GPIO = 0,    // GPIO设备
    DEVICE_TYPE_PWM,         // PWM设备
    DEVICE_TYPE_I2C,         // I2C设备
    DEVICE_TYPE_SPI,         // SPI设备
    DEVICE_TYPE_UART,        // 串口设备
    DEVICE_TYPE_MAX
} DeviceType;

// 设备状态码
typedef enum {
    DEVICE_STATUS_UNKNOWN = -1,   // 未知状态
    DEVICE_STATUS_OFF = 0,        // 关闭状态
    DEVICE_STATUS_ON = 1,         // 开启状态
    DEVICE_STATUS_ERROR = 2       // 错误状态
} DeviceStatus;

// 设备事件类型
typedef enum {
    DEVICE_EVENT_NONE = 0,        // 无事件
    DEVICE_EVENT_STATUS_CHANGE,   // 状态改变
    DEVICE_EVENT_ERROR,           // 错误发生
    DEVICE_EVENT_TIMEOUT         // 超时事件
} DeviceEvent;

// 事件回调函数定义
typedef void (*DeviceEventCallback)(struct Device* dev, DeviceEvent event, void* data);

// 定义每个设备
struct Device
{
    // 基本属性
    DeviceType type;                  // 设备类型
    int fd;                           // 设备描述符
    char deviceName[128];             // 设备名称
    DeviceStatus status;              // 设备状态
    int pinNum;                       // 硬件引脚号
    int triggerMode;                  // 触发方式
    
    // 错误处理
    int errorCode;                    // 错误码
    char errorMsg[256];              // 错误信息
    
    // 设备操作函数
    int (*init)(struct Device* dev);                    // 初始化设备
    int (*deinit)(struct Device* dev);                  // 清理设备资源
    int (*open)(struct Device* dev);                    // 开启设备
    int (*close)(struct Device* dev);                   // 关闭设备
    int (*readStatus)(struct Device* dev);              // 获取设备状态
    void (*setStatus)(struct Device* dev, int status);  // 设置状态
    int (*control)(struct Device* dev, int cmd, void* arg); // 通用控制接口
    
    // 事件处理
    DeviceEventCallback eventCallback;     // 事件回调函数
    void* userData;                        // 用户数据指针
    
    // 同步与链表
    pthread_mutex_t ioLock;               // IO操作互斥锁
    pthread_cond_t statusCond;            // 状态变化条件变量
    struct Device* next;                  // 链表下一节点
};

// 设备创建与管理函数
struct Device* createDevice(DeviceType type, const char* name, int pin);
void destroyDevice(struct Device* dev);
int registerDeviceCallback(struct Device* dev, DeviceEventCallback callback, void* userData);

// 在设备链表中通过设备名字找设备
struct Device* findDeviceByName(struct Device* pdeviceHead, const char* deviceName);

#endif