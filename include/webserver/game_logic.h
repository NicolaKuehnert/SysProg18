#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <temperatur/tempSensor.h>
#include <ledanzeige/TM1637.h>
#include <ledanzeige/segmentanzeige.h>

//#define TEMPO sensor.getTemp() / 50

typedef struct player {
	int curr_face, curr_x, curr_y;
	int points;
	int socket;
	char name;
}player;

void init_game();

float get_tempo();

void move_left(player * pl);

void move_right(player * pl);

void move_forward(player * pl);

#endif
