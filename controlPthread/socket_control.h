#ifndef SOCKET_CONTROL_H
#define SOCKET_CONTROL_H

#include <pthread.h>
#include "command.h"
#include "thread_pool.h"

void handle_client_message(int c_fd, struct InputCommand* socketHandler);
void* socket_thread(void* data);

#endif // SOCKET_CONTROL_H