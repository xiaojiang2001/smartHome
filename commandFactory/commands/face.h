#ifndef __FACE_H
#define __FACE_H

typedef unsigned int bool;

#define true    1
#define false   0

// 图片路径
#define BASIC_PATH "/home/pi/smartHome/commandFactory/commands/"
#define PHOTO_PATH "face0.jpg"

// 云平台信息
#define KEY     "4tcsL5hqeR5yxH65RM1JcR"
#define SECRET  "dde9c7a794604933b1ec47cfda5b3fe0"
#define TYPEID  21
#define FORMAT "json"


#define FACE_DEVICE_NAME "face"

struct faceCommand;  

bool postUrl();

size_t parseDataHandler(void *ptr, size_t size,size_t nmemb, void *stream);

bool parseData(const char *json );

#endif




