#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "webserver/server.h"

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;

void init_server() {
	
	
	
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
			std::cout << std::to_string(l) + "listen\n";
		} else {
			std::cout << "FAIL bind\n";
			std::cout << std::to_string(b) + "\n";
		}
	} else {
		std::cout << "FAIL socket\n";
		std::cout << std::to_string(s) + "\n";
	}
}

int receive() {
	int new_socket, valread;
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	
	if ((new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size))>=0) 
	{ 
		std::cout << std::to_string(new_socket) + "\n";
		valread = read(new_socket, buffer, 1024); 
		std::cout << std::to_string(valread) + "hallo\n";
		send(new_socket , hello , strlen(hello) , 0 ); 
		std::cout << "end";
	} 
	else {
		std::cout << "Fail accept\n";
	}
}


int main() {
	init_server();
	while(true)
	{
		receive();
	}
}

