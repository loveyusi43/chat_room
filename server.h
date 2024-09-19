#ifndef __SERVER_H
#define __SERVER_H

#include "socket.h"

void StartServer(void);

void HandleClient(const ljh::socket::Socket&);

#endif