#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include "webserver/server.h"



void init_server() {
	
	struct sockaddr_un my_addr, peer_addr;
	socklen_t peer_addr_size;
	
	memset(&my_addr, 0, sizeof(struct sockaddr_un)); 
    my_addr.sun_family = AF_INET; 
    strncpy(my_addr.sun_path, "192.168.178.31", sizeof(my_addr.sun_path) - 1);
	
	
	
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s != -1) {
		int b = bind(s, (struct sockaddr *) &my_addr,sizeof(struct sockaddr_un));
		if(b != -1) {
			int l = listen(s, LISTEN_BACKLOG);
			std::cout << std::to_string(l) + "\n";
			
			peer_addr_size = sizeof(struct sockaddr_un);
			int a = accept(s, (struct sockaddr *) &peer_addr, &peer_addr_size);
			std::cout << std::to_string(a) + "\n";
		}
	}
}

