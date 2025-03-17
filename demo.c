// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <wiringSerial.h>
// #include <wiringPi.h>
// #include "pthread.h"
// #include <stdlib.h>

// int fd;

// void *write_thread(void *datas)
// {
//     char send_buf[]="hello world";
//     char cmd;
//     while(1)
//     {
//         printf("pthread1:按任意键发送数据\n");
//         scanf("%c",&cmd);
//         write(fd, send_buf, strlen(send_buf));  //测试发送数据
//     }
// }

// void *read_thread(void *datas)
// {
//     int nread;
//     char msg[128]={'\0'};
//     printf("pthread2:listing......\n");
//     while(1)
//     {
//         while(serialDataAvail(fd) != -1 ) //串口有数据
//         {
//             nread=read(fd, msg,128);    //读取并保存数据
//             if(nread==0)
//                 continue;
//             printf("get data:%d Byte context:%s\n",nread,msg);
//             printf("pthread2:listing......\n");
//             memset(msg,0,strlen(msg));
//         }
//     }
// }

// int main()
// {

//     pthread_t writeThread;
//     pthread_t readThread;


//     if(-1==wiringPiSetup())
//     {
//         printf("setup error\n");
//         exit(-1);
//     }

//     //请开启实验对应的串口即可, 若同时用到多个串口，注意变量命名
//     //fd = serialOpen("/dev/ttyAMA1",9600); //打开ttyAMA1
//     //fd = serialOpen("/dev/ttyAMA2",9600); //打开ttyAMA2
//     //fd = serialOpen("/dev/ttyAMA3",9600); //打开ttyAMA3
//     fd = serialOpen("/dev/ttyAMA4",9600);   //打开ttyAMA4
//     printf("fd:%d\n", fd);

//     pthread_create(&writeThread,NULL, write_thread,NULL);   //创建串口发送线程
//     pthread_create(&readThread, NULL, read_thread, NULL);   //创建串口接收线程

//     pthread_join(writeThread,NULL);
//     pthread_join(readThread, NULL);

//     return 0;
// }
