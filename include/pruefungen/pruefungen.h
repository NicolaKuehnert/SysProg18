#define NAME_LENGTH 50
#define MAXIMUM_POINTS 320

typedef enum degree_program{
	IFM,ELM,Pflege
}degree_program;

//! Ein Student
/*!
Struct Student zum anlegen eines Studenten
@param name Der Name des Studenten
@param cps Die Creditpoints des Studenten
@param enrolled Das Fach des Studenten
*/
typedef struct Student{
	char name[NAME_LENGTH];
	unsigned int cps;
	degree_program enrolled;
}Student;

/*! Funktion zum Registrieren eines Studenten in die Prüfungen

@param Student* Ein Pointer auf einen Studenten
@param int Die ID des Prüfungslots
@return nr Die ID des Prüfungsslots
@return -1 Fehler beim Eintragen
*/
int register_student(Student*,int);

/**
Funktion zum Löschen eines Studenten aus der Prüfungsliste
@param Student* Ein Pointer auf den zu löschenden Studenten
@return i Die ID des Prüfungslots
@return -1 Fehler beim Löschen
*/
int remove_student(Student*);

/**
Gibt den Durchschnitt der CPS aller Studenten zurück
@return float Der Durchschnitt der CPS
*/
float calculate_avarage(void);

/**
Not implemented
*/
void display_avarage(void);

/**
Not implemented
*/
void display_absolute(void);

/**
Initialisiert die Prüfungsliste
*/
void init(void);
