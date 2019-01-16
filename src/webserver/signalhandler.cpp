#include <iostream>
#include <TM1673.h>
#include <csignal>
#include <sqlite_demo.h>
#include "signalhandler.h"
#include "server.h"

I_SQLITE db;


void signalHandler(int signum){
	std::cout << "Signal: " + signum;

	//Handle signals
	switch(signum){
		case SIGINT:
			// Datenbank Zeug auf LED
			break;

		case SIGTERM:
			//Speichern in DB
			save_to_db();
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
