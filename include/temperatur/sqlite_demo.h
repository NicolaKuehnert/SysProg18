#include <sqlite3.h>
#ifndef ISQLITE
#define ISQLITE

class I_SQLite {

	public:
		I_SQLite(char *dbName);
		int exec(char *sql);
		
		~I_SQLite();
	private:
		sqlite3 *db;

};
#endif
