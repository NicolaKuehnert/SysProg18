#define NAME_LENGTH 50
#include "./spfree.h"

typedef enum degree_program{
	IFM,ELM,Pflege
}degree_program;

/**
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

typedef struct node{
	Student *stdnt;
	struct node *next;
	struct node *prev;
}node;

/**

*/
node * get_ifm_students(node*);
void show_all(node*);
node * append_student(node*, Student*);
node* delete_node(node*, sp_purge);
int delete_list(node*);
int delete_list_partial(node*);
node* getAnfang(node*);
