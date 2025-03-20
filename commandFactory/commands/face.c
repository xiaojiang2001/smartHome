#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "face.h"
#include "command.h"
#include <stdlib.h>  
#include <string.h>  

int faceInit(struct InputCommand *faceMes)
{
    // 初始化
    curl_global_init(CURL_GLOBAL_DEFAULT);       // 主线程调用一次
    strcpy(faceMes->command, "./img.jpg");
    return 0;
}

int faceGetCommand()
{
    parseData(); 
    // bool ret = postUrl();
    // if(ret == false) {
    //     printf("curl is null\n");
    //     return false;
    // }
    return true;
}


// 功能函数
char *getBase64(char *filePath)
{
	char *bufPic;
	char cmd[128]={'\0'};
	sprintf(cmd,"base64 %s > tempFile", filePath);
	system(cmd);

	int fd = open("./tempFile", O_RDWR);
    int filelen = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    bufPic = (char *)malloc(filelen+2);
    memset(bufPic, '\0', filelen+2);
    read(fd, bufPic, filelen);
    close(fd);	

	system("rm -f tempFile");
	return bufPic;
}


struct InputCommand faceCommand = {
	.deviceName = FACE_DEVICE_NAME,
	.command = "./img.jpg",
	.log = {'\0'},
	.init = faceInit,
    .getCommand = faceGetCommand,
	.next = NULL
};

// 添加设备到工厂链表
struct InputCommand* addFaceContrlToInputCommandLink(struct InputCommand *phead)
{
	if(phead == NULL)
		return &faceCommand;

	faceCommand.next = phead;
	return &faceCommand;
}

// 解析获取到的字符串 返回识别结果
size_t parseDataHandler(void *ptr, size_t size,size_t nmemb, void *stream)
{
    (void)size;    // 忽略 size 参数  
    (void)nmemb;   // 忽略 nmemb 参数  
    (void)stream;  // 忽略 stream 参数  

    memset(faceCommand.log, '\0', 1024);
    strncpy(faceCommand.log, ptr, 1024); 
    printf("--------------------------get data------------------------------\n");
    printf("buf: %s\n", faceCommand.log); 

    return 0;
}

bool postUrl()
{
    CURL *curl;
    CURLcode res;

    char *postString;
    
    char *bufPic1 = getBase64("./face.jpg");
    char *bufPic2 = getBase64(faceCommand.command);
    char *key = "4tcsL5hqeR5yxH65RM1JcR";
    char *secret = "dde9c7a794604933b1ec47cfda5b3fe0";
    int typeId = 21;
    char *format = "json";

    int len = strlen(key)+strlen(secret)+strlen(bufPic1)+strlen(bufPic2)+128;	
    postString = (char *)malloc(len);
    memset(postString, '\0', len);

    sprintf(postString,"&img1=%s&img2=%s&key=%s&secret=%s&typeId=%d&format=%s",
                        bufPic1, bufPic2, key, secret, typeId, format);

    curl = curl_easy_init();
    if(curl == NULL)
        return false;

    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt");                      // 指定cookie文件
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postString);                             // 指定post内容
    curl_easy_setopt(curl, CURLOPT_URL, "https://netocr.com/verapi/verFaceImage.do");   // 指定url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parseDataHandler); 
    
    res = curl_easy_perform(curl);
    if(res == 0) {
        printf("curl_easy_perform  error\n");
        return false;
    }

    curl_easy_cleanup(curl);

    return true;
}

void parseData()
{
    printf("in parseData");
    // 你的 JSON 字符串  
    const char *json = "{ \"infoList\": [ { \"veritem\": [ { \"desc\": \"判定结果\", \"content\": \"是\" }, { \"desc\": \"判定值\", \"content\": \"98.81272440944882\" } ] } ], \"messageString\": { \"status\": \"2000\", \"value\": \"成功\" } } ] }";  
 
    // 提取 infoList  
    const char *infoListStart = strstr(json, "\"infoList\":");  
    if (infoListStart) 
    {  
        const char *veritemStart = strstr(infoListStart, "\"veritem\":");  
        if (veritemStart) 
        {  
            veritemStart += strlen("\"veritem\":");  
            const char *descStart = strstr(veritemStart, "\"desc\":");
            printf("descStart:", %c);
              
            while (descStart) 
            {  
                descStart += strlen("\"desc\":") + 1;       // Skip over key and quote  
                const char *contentStart = strstr(descStart, "\"content\":");  
                if (contentStart) 
                {  
                    contentStart += strlen("\"content\":") + 1;  

                    // Extract values  
                    size_t descLength = strcspn(descStart, "\"");  
                    size_t contentLength = strcspn(contentStart, "\"");  

                    // Print extracted values  
                    printf("Description: %.*s\n", (int)descLength, descStart);  
                    printf("Content: %.*s\n\n", (int)contentLength, contentStart);  
                }  
                descStart = strstr(contentStart, "\"desc\":");  // Move to next "desc"  
            }  
        }  
        else
            printf("veritemStart is null\n"); 
    }  
    else
        printf("infoListStart is null\n");

    // 提取 messageString  
    const char *messageStart = strstr(json, "\"messageString\":");  
    if (messageStart) 
    {  
        const char *statusStart = strstr(messageStart, "\"status\":");  
        if (statusStart) 
        {  
            statusStart += strlen("\"status\":") + 1;  // Skip over key and quote  
            size_t statusLength = strcspn(statusStart, "\"");  
            printf("Status: %.*s\n", (int)statusLength, statusStart);  
        }  

        const char *valueStart = strstr(messageStart, "\"value\":");  
        if (valueStart) 
        {  
            valueStart += strlen("\"value\":") + 1;  // Skip over key and quote  
            size_t valueLength = strcspn(valueStart, "\"");  
            printf("Value: %.*s\n", (int)valueLength, valueStart);  
        }  
    }  
    else
        printf("messageStart is null\n");
}