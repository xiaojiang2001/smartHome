#ifndef __FACE_H
#define __FACE_H

typedef unsigned int bool;

#define true    1
#define false   0

#define FACE_DEVICE_NAME "face"

struct faceCommand;  

bool postUrl();

size_t parseDataHandler(void *ptr, size_t size,size_t nmemb, void *stream);

void parseData();

// 方法
// int faceInit(struct InputCommand *faceMes);
// int faceGetCommand(struct InputCommand *faceMes);

// // 功能函数
// char *getBase64(char *filePath);
// bool postUrl(char *bufimg);
// size_t parseDataHandler(void *ptr, size_t size,size_t nmemb, void *stream);


#endif




