#include "webserver/game_logic.h"




/**
@param raw Roher Temperaturwert
@return float Die aktuelle Temperatur
*/
float calculateTemperature(int raw){
	float temp = ((float)raw * (3.3/1024) -0.5) * 100;
	return temp;
}

TempSensor sensor = TempSensor(calculateTemperature);

void init_game()
{
	TM1637_setup();
	float geschwindigkeit = sensor.getTemp() / 50;
	
}
