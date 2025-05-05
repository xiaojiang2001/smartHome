#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include "devicesFactory/devices.h"


extern int faceCnt;
extern pthread_mutex_t faceMutex;
extern pthread_cond_t faceNotEmptyMutex;
extern pthread_cond_t faceNotFullMutex;

extern struct Device* pdeviceHead;

void init_common();
void destroy_common();

#endif // COMMON_H