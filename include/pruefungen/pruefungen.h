#define NAME_LENGTH 50
#define MAXIMUM_POINTS 320

typedef enum degree_program{
	IFM,ELM,Pflege
}degree_program;

typedef struct Student{
	char name[NAME_LENGTH];
	unsigned int cps;
	degree_program enrolled;
}Student;



int register_student(Student*,int);
int remove_student(Student*);
float calculate_avarage(void);
void display_avarage(void);
void display_absolute(void);
void init(void);
