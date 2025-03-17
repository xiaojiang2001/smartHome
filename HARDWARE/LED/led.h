#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(13)

//led
#define led3 PBout(12)
#define led1 PBout(13)
#define led2 PBout(14)
#define lock PBout(15)
#define beep PBout(9)

#define roadLED1 PBout(5)
#define roadLED2 PBout(8)


void Command_GPIO_Init(void);//≥ı ºªØ

#endif
