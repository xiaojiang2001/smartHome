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
        if(strcmp(temp->commandName, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}