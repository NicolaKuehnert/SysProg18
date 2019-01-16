#include <iostream>
#include <ledanzeige/TM1637.h>
#include <ledanzeige/segmentanzeige.h>
#include <csignal>
#include "temperatur/sqlite_demo.h"
#include "webserver/signalhandler.h"
#include "webserver/server.h"
#include "webserver/game_logic.h"
#include <csignal>




void signal_save_db_exit(int signum)
{
	save_to_db();
	std::cout << "exit" << std::endl;
	exit(0);
}

void signal_show_led(int signum)
{
	std::cout << "led" << std::endl;
}
void signal_update_game(int signum)
{
	std::string m;
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		// nachricht : "id,currface,curr_x,curr_y,points;"
		m = std::to_string(p->socket) + "," + std::to_string(p->curr_face) + "," + std::to_string(p->curr_x) + "," + std::to_string(p->curr_y) + "," + std::to_string(p->points) + ";";
	}
	send_to_all_clients(m.c_str());
	
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		move_forward(p);
	}
}

void signal_left(int signum)
{
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		move_left(p);
	}
	run_and_send();
}

void signal_right(int signum)
{
	for(int i = 0; i<liste->player_count; i++)
	{
		player *p = liste->list[i];
		move_right(p);
	}
	run_and_send();
}
