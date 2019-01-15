#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include "webserver/server.h"
#include <netdb.h>

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;
player *player_list[10];
int player_count = 0;

static player *add_player(int socket_id);
void accept_connection();
void send_status();

int init_server()
{
	openlog("TronServer",LOG_INFO | LOG_PID, LOG_USER);

	syslog(LOG_INFO, "Server starting");
	memset(&my_addr, 0, sizeof(struct sockaddr_in)); 
    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons( PORT );
    peer_addr_size = sizeof(struct sockaddr_in);
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s != -1) {
		int b = bind(s, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_in));
		if(b != -1) {
			int l = listen(s, LISTEN_BACKLOG);
			syslog(LOG_INFO, "Server started");
			std::cout << "Server running.\n";
			return 0;
		} else {
			syslog(LOG_ERR, "Server crashed - failed binding");
			std::cout << "FAIL bind\n";
			std::cout << std::to_string(b) + "\n";
			return -1;
		}
	} else {
		syslog(LOG_ERR, "Server did not start - socket fail");
		std::cout << "FAIL socket\n";
		std::cout << std::to_string(s) + "\n";
		return -1;
	}
}

void accept_connection()
{
	send_status();
	while (1) {
		std::cout << "waitung for client" << std::endl;
		int new_socket, pid;
		new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size);
		if (new_socket < 0) 
		  {
			 exit(1);
		  }
		  add_player(new_socket);
		  pid = fork();
		  if (pid < 0) 
		  {
			 exit(1);
		  }
		  if (pid == 0) {
			 /* This is the client process */
			 close(s);
			 handle_method(new_socket);
			 exit(0);
		  }
		  else {
			close(new_socket);
		  }
   }
}

void send_status()
{
	int pid = fork();
	if (pid < 0) 
	{
	 exit(1);
	}
	if (pid == 0) 
	{
		while(true)
		{
			std::cout << std::to_string(player_count) << std::endl;
			sleep(1);
			std::cout << "gesendet" << std::endl;
			send_to_all_clients("status");
		}
	}
}

static player *add_player(int socket_id)
{
	player *p = new player;
	p->socket = socket_id;
	player_list[player_count] = p;
	player_count++;
	std::cout << std::to_string(player_count) << std::endl;
	// hier noch position und punkte....
	return p;
}

message *receive_from_client(int c_socket)
{
   int n, len = 1024;
   char buffer[len] = {0};
   n = read(c_socket,buffer,len);
   
   if (n < 0) {
	  std::cout << "fehler beim socket lesen" << std::endl;
	  return nullptr;
   }
   std::cout << buffer << std::endl;
   message *m = new message;
   m->player_id = c_socket;
   m->content= buffer;
   return m;
}

void send_to_client(int c_socket,const char* content) 
{
	send(c_socket, content , strlen(content), 0 ); 
}

void send_to_all_clients(char *content)
{
	for(int i = 0; i<player_count; i++)
	{
		player *p = player_list[i];
		send_to_client(p->socket, content);
	}
}

void handle_method(int c_socket)
{
	while(true)
	{
		message  *m = receive_from_client(c_socket);
		if (m != nullptr)
		{
			//send_to_client(c_socket, "nachricht erhalten");
		}
		else 
		{
			return;
		}
		
	}
}


int main() {
	int running = init_server();
	accept_connection();
	syslog(LOG_INFO, "Server closing...");
	return 0;
}
