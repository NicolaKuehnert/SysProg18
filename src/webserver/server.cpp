#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "webserver/server.h"

int s;
struct sockaddr_in my_addr;

void init_server() {
	
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_size;
	
	memset(&my_addr, 0, sizeof(struct sockaddr_in)); 
    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons( PORT );
	
	
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s != -1) {
		int b = bind(s, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_in));
		if(b != -1) {
			int l = listen(s, LISTEN_BACKLOG);
			std::cout << std::to_string(l) + "listen\n";
			
			peer_addr_size = sizeof(struct sockaddr_in);
			int new_socket;
			if ((new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size))>=0) 
			{ 
				char buffer[1024] = {0}; 
				char *hello = "Hello from server"; 
				int valread = read( new_socket , buffer, 1024); 
				printf("%s\n",buffer ); 
				send(new_socket , hello , strlen(hello) , 0 ); 
			} 
			else {
				std::cout << "Fail accept\n";
			}
		} else {
			std::cout << "FAIL bind\n";
			std::cout << std::to_string(b) + "\n";

		}
	} else {
		std::cout << "FAIL socket\n";
		std::cout << std::to_string(s) + "\n";
	}
}


int main() {
	init_server();
}

