#ifndef GUI_H
#define GUI_H 
#include <ncurses.h>
extern int player_id;

int move();

int init();

void set_position(int x, int y, int direction);

int end();

void send_move(char *direction);

int get_key();

void set_player_id(int id);

#endif
