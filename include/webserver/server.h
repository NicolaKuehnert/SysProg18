#ifndef SERVER_H
#define SERVER_H
#define PORT 8081
#define LISTEN_BACKLOG 50
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "webserver/game_logic.h"
#include "temperatur/sqlite_demo.h"
#include <csignal>

//extern player *player_list[10];
//extern int player_count;
extern I_SQLite db;


typedef struct message{
	int player_id;
	char* content;
}message;

typedef struct player_list{
	player * list[10];
	int player_count = 0; 
}player_list;

extern player_list *liste;

int init_server();
message *receive_from_client(int c_socket);
void send_to_client(int socket, const char *content);
void handle_method(int c_socket);
void send_to_all_clients(const char *content);
int save_to_db();

#endif
