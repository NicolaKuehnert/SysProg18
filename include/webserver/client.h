#ifndef CLIENT_H
#define CLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8081

int init_client();
char *receive_from_server();
void send_to_server(char *content);
void handle_method();


#endif
