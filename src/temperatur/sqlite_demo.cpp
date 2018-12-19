#include <stdio.h>
#include <sqlite3.h>
#include "temperatur/sqlite_demo.h"

/**
Callback Funktion die an SQLite übergeben wird
Verabreitet Daten, die von einer SQL Abfrage zurück kommen
*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	for(int i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}

/**
Konstruktor öffnet eine Datenbank

@param dbName Der Name der Datenbank
*/
I_SQLite::I_SQLite(char *dbName){
	int rc = sqlite3_open(dbName, &db);
	if(rc){
		printf("Could not open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
}

/**
Führt SQL Befehle aus

@param sql Ein SQL String aus einem oder mehreren SQL Befehlen, die mit semicolon getrennt sein müssen
*/
int I_SQLite::exec(char *sql){
	char *zErrMsg = 0;
	int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	
	if(rc != SQLITE_OK){
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 0;
}

/**
Dekonstruktor schließt die Datenbank
*/
I_SQLite::~I_SQLite(){
	sqlite3_close(db);
}


