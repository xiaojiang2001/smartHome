#ifndef _VOICECONTROL_H
#define _VOICECONTROL_H

#define VOICER_DEVICE_NAME "/dev/ttyAMA0"
#define VOICER_DEVICE_BOAD 9600
#define COMMANDNAME "voice"

// 前向声明，而不是具体定义  
struct InputCommand;  
int voiceInit(struct InputCommand *voicer);
int voiceGetCommand(struct InputCommand *voicer);


#endif