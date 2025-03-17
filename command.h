#ifndef _INPUTCOMMAND_H_
#define _INPUTCOMMAND_H_

// 指令工厂

//定义每条指令结构体
struct InputCommand
{
    int fd;                 // 文件描述符
    char commandName[128];  // 指令名称
    char deviceName[128];   // 硬件设备名称
    int boad;               // 硬件波特率
    char command[32];       // 指令内容
    char log[1024];         // 日志
    char ipAddr[32];        // ip地址
    char port[12];          // 端口号
    int (*init)(struct InputCommand *command);          // 初始化指令
    int (*getCommand)(struct InputCommand *command);    // 获取指令

    struct InputCommand* next;       // next指针  
};

// 指令加入指令工厂
struct InputCommand* addSocketContrlToInputCommandLink(struct  InputCommand *phead);
struct InputCommand* addVoiceContrlToInputCommandLink(struct InputCommand *phead);

// 通过指令名称找到对应的指令节点
struct InputCommand* findCommandByNanme(struct InputCommand* head, char * name);

#endif
