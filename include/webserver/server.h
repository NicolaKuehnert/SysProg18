#ifndef SERVER_H
#define SERVER_H

#define LISTEN_BACKLOG 50
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



void init_server();

#endif
