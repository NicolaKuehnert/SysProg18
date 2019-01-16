#include <iostream>
#include <TM1673.h>
#include <csignal>
#include <sqlite_demo.h>
#include "signalhandler.h"

I_SQLITE db;

int init_handler(){
	db = I_SQLITE("tronserver.db");
	/*
		DB Layout:
			gameboard(id INTEGER PRIMARY KEY, datum TEXT DEFAULT CURRENT_TIMESTAMP, spieldauer TEXT)
			spieler(name TEXT, score TEXT, game INTEGER FOREIGN KEY gameboard(id))
	*/

}

void signalHandler(int signum){
	std::cout << "Signal: " + signum;

	//Handle signals
	switch(signum){
		case SIGINT:
			// Datenbank Zeug auf LED
			break;

		case SIGTERM:
			//Speichern in DB
			db.exec("");
			break;

		case SIGHUP:
			//Spielfeld neu lesen
			break;

		case SIGUSR1:
			//Takzeit runter
			break;

		case SIGUSR2:
			//Taktzeit hoch
			break;
	}

}
