#ifndef SERVER_H
#define SERVER_H

#define LISTEN_BACKLOG 50

typedef struct sockaddr_un { 
    sa_family_t sun_family; 
    char        sun_path[14]; 
} sockaddr_un;

void init_server();

#endif
