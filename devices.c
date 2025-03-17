#include "devices.h"
#include <stdio.h>
#include <string.h>

// 通过设备名字找设备
struct Device* findDeviceByNanme(struct Device* pdeviceHead, char *deviceName)
{
    struct Device* temp = pdeviceHead;
    if(NULL == pdeviceHead){
        printf("pdeviceHead is null!!!\n");
        return NULL;
    }
    while (temp != NULL)
    {
        // 找到名字一样的设备了
        if(strcmp(temp->deviceName, deviceName) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    // 没有这个名字的设备
    return NULL;
}