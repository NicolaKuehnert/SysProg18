#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <temperatur/tempSensor.h>
#include <ledanzeige/TM1637.h>
#include <ledanzeige/segmentanzeige.h>
typedef struct player {
	int curr_face, curr_x, curr_y;
	int points;
	int socket;
}player;

#endif
