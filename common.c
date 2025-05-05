#include "common.h"

int faceCnt = 0;
pthread_mutex_t faceMutex;
pthread_cond_t faceNotEmptyMutex;
pthread_cond_t faceNotFullMutex;

struct Device* pdeviceHead = NULL;

void init_common() {
    pthread_mutex_init(&faceMutex, NULL);
    pthread_cond_init(&faceNotEmptyMutex, NULL);
    pthread_cond_init(&faceNotFullMutex, NULL);
}

void destroy_common() {
    pthread_mutex_destroy(&faceMutex);
    pthread_cond_destroy(&faceNotEmptyMutex);
    pthread_cond_destroy(&faceNotFullMutex);
}