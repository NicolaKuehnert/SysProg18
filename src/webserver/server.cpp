#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include "webserver/server.h"

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;
static player *player_list[10];
static int player_count = 0;

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

message *receive_from_client() {
	int new_socket;
	char buffer[1024] = {0}; 
	
	if ((new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size))>=0) 
	{ 
		read(new_socket, buffer, 1024); 
		std::cout << buffer << std::endl;
		message * m= new message;
		m->content = buffer;
		m->player_id = new_socket;
		syslog(LOG_INFO, "Client transmission recieved");
		return m;
	} 
	else {
		std::cout << "Fail receive\n";
		syslog(LOG_INFO, "Client transmission failed to recieve");
	}
	return nullptr;
}


void handle_method()
{
	sleep(1);
	send_to_all_clients("status");
	std::cout << "waitung" << std::endl;
	message *command = receive_from_client();
	if(strcmp(command->content, "new")==0)
	{
		//syslog("New Client connected");
		std::cout << "new client" <<std::endl;
		player *p = new player;
		p->socket = command->player_id;
		//p->curr_face = 0;  // position setzen !!!
		//p->curr_x = 0;
		//p->curr_y = 0;
		
		player_list[player_count] = p;
		player_count++;
		send_to_client(command->player_id, std::to_string(command->player_id).c_str());
		syslog(LOG_INFO, "New Client connected: %i", command->player_id);
	}
	else if (strcmp(command->content, "l")==0)
	{
		syslog(LOG_INFO, "Client transmission -- turn left");
		send_to_client(command->player_id, "empfangen l");
	}
	else if (strcmp(command->content, "r")==0)
	{
		syslog(LOG_INFO, "Client transmission -- turn right");
		send_to_client(command->player_id, "empfangen r");
	}
	else if (strcmp(command->content, "f")==0)
	{
		send_to_client(command->player_id, "empfangen f");
	}
}


void send_to_client(int c_socket,const char* content) 
{
	send(c_socket, content , strlen(content), 0 ); 
}

void write_log(){

}

void send_to_all_clients(char *content)
{
	for(int i = 0; i<player_count; i++)
	{
		player *p = player_list[i];
		send_to_client(p->socket, content);
	}
}


int main() {
	int running = init_server();
	while (running >= 0)
	{
		handle_method();
	}
	//close(s);
	syslog(LOG_INFO, "Server closing...");
	return 0;
}

