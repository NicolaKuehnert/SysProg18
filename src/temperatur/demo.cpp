#include "temperatur/calcTemp.h"
#include "temperatur/tempSensor.h"
#include "temperatur/sqlite_demo.h"
#include <ringbuffer/ringbuffer.h>
#include <ledanzeige/TM1637.h>
#include <ledanzeige/segmentanzeige.h>
#include "temperatur/curl_api.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <string>



using namespace std;

#define LOOPCOUNT 5


int main(){
	//ring_buffer* buffer = init_buffer(10, free);

	I_SQLite db = I_SQLite("temperatur.db");

	if(db.exec("CREATE TABLE temperatures(time text DEFAULT CURRENT_TIMESTAMP, temperature text);") == 0);

	TempSensor sensor = TempSensor(calculateTemperature);
	int runs = 0;
	TM1637_setup();


	//SQL Strings zum Speichern der Temperatur
	std::string sql = "INSERT INTO temperatures(Itemperature) VALUES(";
	std::string sql2 = ");";
	

	test();

	while(true){
		runs++;
		
		
		float temp = sensor.getTemp();
		sql += temp;
		sql += sql2;
		char *ptr = &sql[0u];
		db.exec(ptr);
		cout << std::to_string(temp) + "\n";
		//write_buffer(buffer, &temp);
		//TM1637_display_number(percent(buffer));
		TM1637_display_number(temp);

		sleep(10);
		
		
	}
	
	return 0;
}
