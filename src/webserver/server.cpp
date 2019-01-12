#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "webserver/server.h"

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;
int player_id;

void init_server() 
{
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
			std::cout << "Server running.\n";
		} else {
			std::cout << "FAIL bind\n";
			std::cout << std::to_string(b) + "\n";
		}
	} else {
		std::cout << "FAIL socket\n";
		std::cout << std::to_string(s) + "\n";
	}
}

char *receive_from_client() {
	int new_socket;
	char buffer[1024] = {0}; 
	
	if ((new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size))>=0) 
	{ 
		read(new_socket, buffer, 1024); 
		std::cout << buffer << std::endl;
		player_id = new_socket;
		char *b = buffer;
		return b;
	} 
	else {
		std::cout << "Fail receive\n";
	}
	return nullptr;
}


void handle_method(const char *command)
{
	if(strcmp(command, "new")==0)
	{
		std::cout << "new client" <<std::endl;
		send_to_client(player_id, std::to_string(player_id).c_str());
	}
	else if (strcmp(command, "l")==0)
	{
		
	}
	else if (strcmp(command, "r")==0)
	{
		
	}
	else if (strcmp(command, "f")==0)
	{
		
	}
}


void send_to_client(int c_socket,const char* content) 
{
	send(c_socket, content , strlen(content), 0 ); 
}




int main() {
	init_server();
	while (true)
	{
		handle_method(receive_from_client());
	}
	//close(s);
	return 0;
}

