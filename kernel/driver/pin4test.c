#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd;
    int cmd;
    
    fd = open("/dev/pin4",O_RDWR);
    if(fd < 0){
        printf("open error\n");
        perror("reason:");
    }
    else
        printf("open success\n");

    printf("please input command:1/0:\n");
    scanf("%d", &cmd);
    
    write(fd, &cmd, 1);
}

