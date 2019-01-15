#ifndef SERVER_H
#define SERVER_H
#define PORT 8081
#define LISTEN_BACKLOG 50
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "webserver/game_logic.h"

extern player *player_list[10];
extern int player_count;

typedef struct message{
	int player_id;
	char* content;
}message;

int init_server();
message *receive_from_client(int c_socket);
void send_to_client(int socket, const char *content);
void handle_method(int c_socket);
void send_to_all_clients(const char *content);

#endif
