#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include "webserver/server.h"
#include <netdb.h>
#include <csignal>
#include <signal.h>

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;
player_list *liste = new player_list;
bool running = false;

int fork_id_send;

static int add_player(int socket_id);
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
			listen(s, LISTEN_BACKLOG);
			syslog(LOG_INFO, "Server started");
			std::cout << "Server running.\n";
			init_game();
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
	while (1) {
		std::cout << "waitung for client" << std::endl;
		int new_socket, pid;
		new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size);
		if (new_socket < 0) 
		  {
			 exit(1);
		  }
		  
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
			  liste->player_count = add_player(new_socket);
			  send_status();
			  close(new_socket);
		  }
		  
   }
}

void send_status()
{
	if(running)
	{
		//kill(fork_id_send,SIGKILL);
		return;
	}
	int pid = fork();
	if (pid < 0) 
	{
	 exit(1);
	}
	if (pid == 0) 
	{
		while(true)
		{
			sleep(1);
			std::cout << "gesendet" << std::endl;
			std::string m;
			for(int i = 0; i<liste->player_count; i++)
			{
				player *p = liste->list[i];
				// nachricht : "id,currface,curr_x,curr_y,points;"
				m = std::to_string(p->socket) + "," + std::to_string(p->curr_face) + "," + std::to_string(p->curr_x) + "," + std::to_string(p->curr_y) + "," + std::to_string(p->points) + ";";
			}
			send_to_all_clients(m.c_str());
			
			for(int i = 0; i<liste->player_count; i++)
			{
				player *p = liste->list[i];
				move_forward(p);
			}
		}
	}
	else
	{
		fork_id_send = pid;
	}
	running = true;
}

static int add_player(int socket_id)
{
	player *p = new player;
	p->socket = socket_id;
	p->points = 0;
	p->curr_face = 0;
	p->curr_x = 5;
	p->curr_y = 5;
	liste->list[liste->player_count++] = p;
	return liste->player_count;
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

void send_to_all_clients(const char *content)
{
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		send_to_client(p->socket, content);
	}
}

player* get_player_by_id(int id)
{
	player *pl;
	for(int i = 0; i<liste->player_count; i++)
	{
		pl = liste->list[i];
		if(pl->socket == id)
		{
			return pl;
		}
	}
	return nullptr;
}

void handle_method(int c_socket)
{
	while(true)
	{
		message  *m = receive_from_client(c_socket);
		if (m != nullptr)
		{
			player *p = get_player_by_id(m->player_id);
			if(p == nullptr)
			{
				add_player(c_socket);
				p = get_player_by_id(m->player_id);
			}
			if(strcmp(m->content, "close") == 0)
			{
				send_to_client(m->player_id, "close");
				close(m->player_id);
			}
			else if(strcmp(m->content, "get_id") == 0)
			{
				std::cout << m->player_id << std::endl;
				send_to_client(m->player_id, std::to_string(m->player_id).c_str());
			}
			else if(strcmp(m->content, "l") == 0)
			{
				move_left(p);
			}
			else if(strcmp(m->content, "r") == 0)
			{
				move_right(p);
			}
		}
		else 
		{
			return;
		}
		
	}
}


int main() {
	//signal(SIGINT, signalHandler); 
	init_server();
	accept_connection();
	syslog(LOG_INFO, "Server closing...");
	return 0;
}
