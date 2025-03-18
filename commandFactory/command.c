#include "command.h"
#include <stdio.h>
#include <string.h>

struct InputCommand* findCommandByNanme(struct InputCommand* head, char * name)
{
    if(head == NULL){
        printf("findCommandByNanme: head is null\n");
        return NULL;
    }
    struct InputCommand* temp = head;
    while (temp)
    {
        if(strcmp(temp->deviceName, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// 分割收到的内容为设备+指令
int split_string(const char *input, char *part1, char *part2, const char delimiter) {  
    // 查找分隔符的位置  
    const char *delimiter_ptr = strchr(input, delimiter);  
    if (delimiter_ptr == NULL) {  
        return -1;  
    }  

    // 计算 part1 的长度  
    size_t len1 = delimiter_ptr - input;  

    // 复制 part1  part2
    strncpy(part1, input, len1);  
    part1[len1] = '\0';                 // 添加字符串结束符  
    strcpy(part2, delimiter_ptr + 1);   // +1 跳过分隔符  

    return 0; 
}  