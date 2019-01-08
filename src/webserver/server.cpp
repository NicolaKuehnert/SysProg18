#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "webserver/server.h"



void init_server() {
	
	struct sockaddr_in my_addr, peer_addr;
	socklen_t peer_addr_size;
	
	memset(&my_addr, 0, sizeof(struct sockaddr_in)); 
    my_addr.sin_family = AF_INET; 
    strncpy(my_addr.sin_addr.s_addr, "192.168.178.31", sizeof("192.168.178.31") - 1);
	
	
	
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s != -1) {
		int b = bind(s, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_in));
		if(b != -1) {
			int l = listen(s, LISTEN_BACKLOG);
			std::cout << std::to_string(l) + "\n";
			
			peer_addr_size = sizeof(struct sockaddr_in);
			int a = accept(s, (struct sockaddr *) &peer_addr, &peer_addr_size);
			std::cout << std::to_string(a) + "\n";
		} else {
			std::cout << "FAIL bind\n";
			std::cout << std::to_string(b) + "\n";

		}
	} else {
		std::cout << "FAIL socket\n";
		std::cout << std::to_string(s) + "\n";
	}
}

