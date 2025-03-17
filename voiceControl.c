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
	int fd;
	if(fd = serialOpen(voicer->deviceName, voicer->boad) == ERROR) {
        printf("voiceInit error!\n");
		exit(-1);
	}
	voicer->fd = fd;
	return fd;
}

int voiceGetCommand(struct InputCommand *voicer)
{
	int nread = 0;
	memset(voicer->command, '\0', sizeof(voicer->command));

	nread = read(voicer->fd, voicer->command, sizeof(voicer->command));
	if(nread == 0){
        printf("uart for voice over time\n");
        return 0;
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