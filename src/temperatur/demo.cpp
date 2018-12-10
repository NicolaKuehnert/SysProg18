#include "temperatur/tempSensor.h"
#include "temperatur/calcTemp.h"
#include <string>
#include <sstream>
#include "temperatur/sqlite_demo.h"
#include "ringbuffer/ringbuffer.h"
#include "ledanzeige/TM1637.h"

#define LOOPCOUNT 5


int main(){

	ring_buffer buffer = init_buffer(10, free);
/*
	I_SQLite db = I_SQLite("temperatur.db");

	if(db.exec("CREATE TABLE temperatures(time text DEFAULT CURRENT_TIMESTAMP, temperature text);") == 0);
*/

	int runs = 0;
	tempSensor sensor = tempSensor(calculateTemperatur);
	TM1637_setup();

/*
	//SQL Strings zum Speichern der Temperatur
	std::string sql = "INSERT INTO temperatures(time, Itemperature) VALUES(";
	std::string sql2 = ");";
*/
	while(true){
		runs++;
		
		float temp = sensor.getTemp();
		write_buffer(buffer, temp);
		TM1637_display_number(percent(buffer));

		cout << temp;

		sleep(10);
		
		
	}
	return 0;
}
