#include "webserver/client.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
struct sockaddr_in address; 
int sock = 0; 
struct sockaddr_in serv_addr; 

int init_client() 
{
	    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
		std::cout << "\n Socket creation error \n"; 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)>0)  
    { 
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
		{ 
			std::cout << "Connection Failed\n"; 
			return -1; 
		} 
		send_to_server("new");
		char* player_id = receive_from_server();
		return std::stoi(player_id);
    } 
    else {
		std::cout << "Invalid Adress\n";
	}
	return -1;
}

void send_to_server(char *content)
{
	send(sock, content, strlen(content), 0 ); 
}

char *receive_from_server()
{
	char buffer[1024] = {0}; 
	read(sock, buffer, 1024); 
	std::cout << buffer;
	std::cout << "\n";
	return buffer;
}
