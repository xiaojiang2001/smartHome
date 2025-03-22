#ifndef VOICE_CONTROL_H
#define VOICE_CONTROL_H

#include <pthread.h>
#include "command.h"

void* voice_thread(void* data);

#endif // VOICE_CONTROL_H