#include "webserver/game_logic.h"
#include <iostream>


float takt = 500.0;

/**
@param raw Roher Temperaturwert
@return float Die aktuelle Temperatur
*/
float calculateTemperature(int raw){
	//float temp = ((float)raw * (3.3/1024) -0.5) * 100;
	//return temp;
}


//TempSensor sensor = TempSensor(calculateTemperature);

/*
Initialisiert die Logik
*/
void init_game()
{
	//TM1637_setup();
	//float geschwindigkeit = TEMPO;
}

/*
Berechnet die Geschwindigkeit anhand der Raumtemperatur
*/
float get_tempo()
{
	//return ((sensor.getTemp() / 5) + takt);
}

/*
Bewegt einen Spieler nach links
*/
void move_left(player * pl) {
	switch(pl->curr_face){
		case 0:
			pl->curr_face = 1;
			break;
		case 1:
			pl->curr_face = 2;
			break;
		case 2:
			pl->curr_face = 3;
			break;
		case 3:
			pl->curr_face = 0;
			break;
		}
}

/*
Bewegt einen Spieler nach rechts
*/
void move_right(player * pl) {
	if(pl->curr_face == 0){
			pl->curr_face = 3;
	}else if(pl->curr_face == 3){
			pl->curr_face = 2;
	}else if(pl->curr_face == 2){
			pl->curr_face = 1;
	}else if(pl->curr_face == 1){
			pl->curr_face = 0;
	}
}

/*
Bewegt einen Spieler vorwärts
*/
void move_forward(player * pl) {
	switch(pl->curr_face){
		case 0:
			pl->curr_x++;
			break;
		case 1:
			pl->curr_y++;
			break;
		case 2:
			pl->curr_x--;
			break;
		case 3:
			pl->curr_y--;
			break;
	}
}
