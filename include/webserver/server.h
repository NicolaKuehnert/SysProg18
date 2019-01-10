#ifndef SERVER_H
#define SERVER_H
#define PORT 8080
#define LISTEN_BACKLOG 50
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



void init_server();
char *receive_from_client();
void send_to_client(char *content);

#endif