#include "command.h"
#include <stdio.h>
#include "voiceControl.h"
#include "wiringPi.h"
#include "defs.h"
#include <string.h>
#include <stdlib.h>
#include<wiringSerial.h>
#include <unistd.h>


struct InputCommand voicer = {
     .commandName = COMMANDNAME,
     .deviceName = VOICER_DEVICE_NAME,
     .boad = VOICER_DEVICE_BOAD,
     .command = '\0',
     .log = '\0',
     .init = voiceInit,
     .getCommand = voiceGetCommand,
     .next = NULL
};

int voiceInit(struct InputCommand *voicer)
{
	int fd = 10;
	fd = serialOpen(voicer->deviceName, voicer->boad);
    if(fd < 0) {
        printf("voiceInit error!\n");
		exit(-1);
	}
	voicer->fd = fd;
	return fd;
}

int voiceGetCommand(struct InputCommand *voicer)
{
	int nread = 0;
    char buf[32];
	memset(buf, '\0', sizeof(buf));
    memset(voicer->commandName, '\0', sizeof(voicer->command));
    memset(voicer->command, '\0', sizeof(voicer->command));

    nread = read(voicer->fd, buf, sizeof(buf));
    if(nread <= 0)
        return -1;
    
    if(split_string(buf, voicer->commandName, voicer->command, '-') < 0){
		printf("split_string error\n");
		return -1;
	}
	return nread;
}

struct InputCommand* addVoiceContrlToInputCommandLink(struct InputCommand *phead)
{
	if(phead == NULL)
		return &voicer;
    voicer.next = phead;
    return &voicer;
}