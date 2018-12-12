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

	if(db.exec("CREATE TABLE Itemperatures(time text DEFAULT CURRENT_TIMESTAMP, temperature text);") == 0);
	if(db.exec("CREATE TABLE Atemperatures(time text DEFAULT CURRENT_TIMESTAMP, Atemperature text, Itemperatur text);") == 0);

	TempSensor sensor = TempSensor(calculateTemperature);
	int runs = 0;
	TM1637_setup();


	//SQL Strings zum Speichern der Temperatur
	std::string sql = "INSERT INTO Itemperatures(temperature) VALUES(";
	std::string sql2 = "INSERT INTO Atemperatures(Atemperature, Itemperatur) VALUES(";
	

	test();

	while(true){
		runs++;

		std::string insert = "";
		
		float temp = sensor.getTemp();
		insert += sql;
		insert += "'";
		insert += temp;
		insert += "');";
		char *ptr = &insert[0u];
		db.exec(ptr);

		/*
		if(runs == LOOPCOUNT){
			//hier dann die Außentemperatur in die DB
			runs = 0;
			int items = count_elements(buffer);
			float alltemps = 0;

			for(int i = 0; i<items; i++){
				//alltemps+=(float*)read_buffer(buffer);   //Wie castet man das zu float?
			}
			float mid = alltemps/(float)items;

			insert = "";
			insert += sql2;
			insert += "AusenTemp"; //Hier kommt der Wert aus der XML später rein
			insert+= ",";
			insert+=mid;
			insert += ");";
			char *ptr2 = &insert[0u];
			db.exec(ptr2);
		}
		*/

		cout << std::to_string(temp) + "\n";
		//write_buffer(buffer, &temp);
		//TM1637_display_number(percent(buffer));
		TM1637_display_number(temp);

		sleep(10);
		
		
	}
	
	return 0;
}
