#include "temperatur/calcTemp.h"
#include "temperatur/tempSensor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
using namespace std;


//#include "temperatur/sqlite_demo.h"

#define LOOPCOUNT 5


int main(){
/*
	I_SQLite db = I_SQLite("temperatur.db");

	if(db.exec("CREATE TABLE temperatures(time text DEFAULT CURRENT_TIMESTAMP, temperature text);") == 0);
*/
	calculateTemperature(1.1);
	int runs = 0;
	TempSensor sensor = TempSensor(calculateTemperature);

/*
	//SQL Strings zum Speichern der Temperatur
	std::string sql = "INSERT INTO temperatures(time, Itemperature) VALUES(";
	std::string sql2 = ");";
*/
	while(true){
		runs++;
		
		float temp = sensor.getTemp();

		cout << temp;

		sleep(10);
		
		
	}
	return 0;
}
