#ifndef GUI_H
#define GUI_H 
#include <ncurses.h>


typedef struct player {
	int curr_face, curr_x, curr_y;
	int points;
}player;

int move();

int init();

void move_forward(player * pl);

void move_left(player * pl);

void move_right(player * pl);

void set_position(player *pl);

int end();

#endif
