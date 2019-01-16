#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include "webserver/server.h"
#include <netdb.h>
#include <csignal>
#include <signal.h>

#include "webserver/signalhandler.h"

int s;
struct sockaddr_in my_addr;
struct sockaddr_in peer_addr;
socklen_t peer_addr_size;
player_list *liste = new player_list;
bool running = false;
int db_game_id = 0;
I_SQLite db = I_SQLite("tronserver.db");

int fork_id_send = 0;

static int add_player(int socket_id);
void accept_connection();
void send_status();

/*
Inititalisiert den Server und startet die Datenbank
*/
int init_server()
{
	
	/*
		DB Layout:
			gameboard(id INTEGER PRIMARY KEY, datum TEXT DEFAULT CURRENT_TIMESTAMP, spieldauer TEXT)
			spieler(name TEXT, score TEXT, game INTEGER FOREIGN KEY gameboard(id))
	*/

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
				//std::cout << "Server running.\n";
				return 1;
		} else {
			syslog(LOG_ERR, "Server crashed - failed binding");
			//std::cout << "FAIL bind\n";
			//std::cout << std::to_string(b) + "\n";
			return 0;
		}
	} else {
		syslog(LOG_ERR, "Server did not start - socket fail");
		//std::cout << "FAIL socket\n";
		//std::cout << std::to_string(s) + "\n";
		return 0;
	}
}

/*
Wartet auf neue Clients und stellt eine Verbindung her
*/
void accept_connection()
{
	while (1) {
		//std::cout << "waitung for client" << std::endl;
		int new_socket, pid;
		new_socket = accept(s, (struct sockaddr *)&my_addr, (socklen_t*)&peer_addr_size);
		if (new_socket < 0) 
		  {
			 exit(1);
		  }
		  liste->player_count = add_player(new_socket);
		  send_status();
		  pid = fork();
		  if (pid < 0) 
		  {
			 exit(1);
		  }
		  if (pid == 0) {
			 /* This is the client process */
			 syslog(LOG_INFO, "Neuer Child Prozess");
			 close(s);
			 handle_method(new_socket);
			 exit(0);
		  }
		  else {
			  close(new_socket);
		  }
		  
   }
}

/*
Senden den aktuellen Status des Spiels an die Clients
*/
void send_status()
{
	if(running)
	{
		return;
	}
	int pid = fork();
	if (pid < 0) 
	{
	 exit(1);
	}
	if (pid == 0) 
	{
		run_and_send();
	}
	else
	{
		fork_id_send = pid;
	}
	running = true;
}

void run_and_send()
{
	while(true)
	{
		/*syslog(LOG_INFO, "bevor temo"); // sollte bleiben, als geschwingikeit
		int i = (int)get_tempo();
		std::cout << i << std::endl;
		syslog(LOG_INFO, "Temperatur: ");
		syslog(LOG_INFO, std::to_string(i).c_str());*/
		sleep(1);

		raise(SIGHUP);
	}
}

/*
Fügt einen Spieler dem Spiel hinzu
*/
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

/*
Emfängt eine Nachricht eines Clients
*/
message *receive_from_client(int c_socket)
{
   int n, len = 1024;
   char buffer[len] = {0};
   n = read(c_socket,buffer,len);
   
   if (n < 0) {
	  //std::cout << "fehler beim socket lesen" << std::endl;
	  return nullptr;
   }
   //std::cout << buffer << std::endl;
   message *m = new message;
   m->player_id = c_socket;
   m->content= buffer;
   return m;
}

/*
Sendet eine Nachricht an einen Client
*/
void send_to_client(int c_socket,const char* content) 
{
	send(c_socket, content , strlen(content), 0 ); 
}

/*
Sendet eine Nachricht an alle Clienten
*/
void send_to_all_clients(const char *content)
{
	syslog(LOG_INFO, "send to all");
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		send_to_client(p->socket, content);
	}
}

/*
Liefert einen Spieler an hand seiner ID zurück
*/
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

/*
Empfängt eine Nachricht vom Client, wertet diese aus und führt Aktionen anhand der Auswertung aus
*/
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
				kill(fork_id_send,SIGINT);
			}
			else if(strcmp(m->content, "get_id") == 0)
			{
				//std::cout << m->player_id << std::endl;
				send_to_client(m->player_id, std::to_string(m->player_id).c_str());
			}
			else if(strcmp(m->content, "l") == 0)
			{
				kill(fork_id_send,SIGUSR1);
				//move_left(p);
			}
			else if(strcmp(m->content, "r") == 0)
			{
				kill(fork_id_send,SIGUSR2);
				//move_right(p);
			}
		}
		else 
		{
			return;
		}
	}
}

/*
Speichert die Spielinformationen in die DB
*/
int save_to_db(){
	
	std::string out = "INSERT INTO gameboard(id, spieldauer) VALUES(";
	for(int i = 0; i < liste->player_count; i++){
		out += std::to_string(db_game_id) + ",";
		out += std::to_string(10) + ");";
	}
	syslog(LOG_INFO, out.c_str());
	char *ptr = &out[0u];
	db.exec(ptr);

	out = "INSERT INTO spieler(name, score, game) VALUES(";

	for(int i = 0; i < liste->player_count; i++){
		out += std::to_string(liste->list[i]->name) + ",";
		out += std::to_string(liste->list[i]->points) + ",";
		out += std::to_string(db_game_id) + ");";
	}
	db.exec(ptr);
		
}


int main() {
	signal(SIGINT, signal_save_db_exit); 
	signal(SIGHUP, signal_update_game); 
	signal(SIGTERM, signal_show_led); 
	signal(SIGUSR1, signal_left); 
	signal(SIGUSR2, signal_right); 
	if(init_server())
	{
		accept_connection();
	}
	syslog(LOG_INFO, "Server closing...");
	return 0;
}
