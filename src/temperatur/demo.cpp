#include "temperatur/calcTemp.h"
#include "temperatur/tempSensor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

using namespace std;


//#include "temperatur/sqlite_demo.h"

#define LOOPCOUNT 5


int main(){
/*
	I_SQLite db = I_SQLite("temperatur.db");

	if(db.exec("CREATE TABLE temperatures(time text DEFAULT CURRENT_TIMESTAMP, temperature text);") == 0);
*/
	//cout << TempSensor::isInitialized; 
	TempSensor sensor = TempSensor(calculateTemperature);
	int runs = 0;
/*
	//SQL Strings zum Speichern der Temperatur
	std::string sql = "INSERT INTO temperatures(time, Itemperature) VALUES(";
	std::string sql2 = ");";
*/	

	while(true){
		runs++;
		cout << sensor.getTemp();
		cout << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		
		
	}
	
	return 0;
}
