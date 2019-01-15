#include "webserver/client.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "webserver/gui.h"

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
		//init();
		send_to_server("get_id");
		
		char* id = receive_from_server();
		set_player_id(atoi(id));
		handle_method();
		
		return 0;
    } 
    else {
		std::cout << "Invalid Adress\n";
	}
	return -1;
}

void handle_method()
{
	int running = 1;
	while (running)
	{
		char * input = receive_from_server();
		if(strcmp(input, "status")==0)
		{
			//update das spiel
		} 
		else {
			//was weiß ich
		}
		//running = get_key();
	}
}

void send_to_server(char *content)
{
	int i = send(sock, content, strlen(content), 0 ); 
}

char *receive_from_server()
{
	int len = 1024;
	char buffer[len] = {0}; 
	recv(sock, buffer, len, 0); 
	std::cout << buffer << std::endl;
	
	char* s = new char[len + 1];
	
	strncpy(s, (char*)buffer, len);
    s[len] = '\0';
	return s;
}
