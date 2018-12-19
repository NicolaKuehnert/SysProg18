#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pruefungen/pruefungen.h"
#include "ledanzeige/TM1637.h"
#include "ledanzeige/segmentanzeige.h"
#include <unistd.h>
Student * exams [10];

int main(){
	float avg;
	int true=0;

	
	

	Student* s1=(Student*)malloc(sizeof(Student));
	Student* s2=(Student*)malloc(sizeof(Student));
	Student* s3=(Student*)malloc(sizeof(Student));
	Student* s4=(Student*)malloc(sizeof(Student));
	Student* s5=(Student*)malloc(sizeof(Student));
	init();
	strcpy(s1->name,"Peter");
	s1->cps = 20;
	s1->enrolled = IFM;
	
	strcpy(s2->name,"Caro");
	s2->cps = 25;
	s2->enrolled = IFM;
	
	strcpy(s3->name,"Jan");
	s3->cps = 0;
	s3->enrolled = Pflege;
	
	strcpy(s5->name,"Tim");
	s5->cps = 20;
	s5->enrolled = ELM;
	
	strcpy(s4->name,"Karl");
	s4->cps = 20;
	s4->enrolled = IFM;

	true=register_student(s1,1);
	printf("%d\n",true);
	true=register_student(s2,1);
	printf("%d\n",true);
	true=register_student(s3,1);
	printf("%d\n",true);
	true=register_student(s4,1);
	printf("%d\n",true);
	
	
	avg=calculate_avarage();
	printf("Avarage: %f\n",avg);

	TM1637_setup();
	TM1637_display_number(avg);
	
	true=remove_student(s3);
	printf("Student an %d entfernt\n",true);
	true=register_student(s5,1);
	printf("%d\n",true);
	
	sleep(10);
	TM1637_clear_display();
	return 0;
}

/**
Funktion zum Registrieren eines Studenten in die Prüfungen
@param Student* Ein Pointer auf einen Studenten
@param int Die ID des Prüfungslots
@return nr Die ID des Prüfungsslots
@return -1 Fehler beim Eintragen
*/
int register_student(Student* s, int nr){
	if(nr>=0||nr<10){
		if(exams[nr]==NULL){
			exams[nr]=s;
		}
		else{
			return register_student(s,nr+1);
		}
		return nr;
	}
	return -1;
}

/**
Funktion zum Löschen eines Studenten aus der Prüfungsliste
@param Student* Ein Pointer auf den zu löschenden Studenten
@return i Die ID des Prüfungslots
@return -1 Fehler beim Löschen
*/	
int remove_student(Student* s){
	int i=0;
	for(i=0;i<10;i++){
			if(exams[i]==s){
					exams[i]=NULL;
					return i;
				}
		}
	return -1;
}
	
/**
Gibt den Durchschnitt der CPS aller Studenten zurück
@return float Der Durchschnitt der CPS
*/
float calculate_avarage(void){
	float av=0.0;
	int ges=0;
	float count=0.0;
	int i;
	for(i=0;i<10;i++){
			if(exams[i] != NULL){
				ges += exams[i]->cps;
				count++;
			}
		}
	av=ges/count;
	
	return av;
}

/**
Initialisiert die Prüfungsliste
*/
void init(void){
	int i=0;
	while(i<10){
		exams[i]=NULL;
		i++;
	}
}























