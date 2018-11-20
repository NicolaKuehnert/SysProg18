#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "studiverwaltung/studiverwaltung.h"


int main(){
	
	/*
	 * Initialisierung Student={Name,cps,enrolled}
	 * enrolled[0]=IFM
	 * enrolled[1]=ELM
	 * enrolled[2]=Pflege
	 */
	node* liste,*info;
	int anzahl=0;
	student* s1=(student*)malloc(sizeof(student));
	student* s2=(student*)malloc(sizeof(student));
	student* s3=(student*)malloc(sizeof(student));
	student* s4=(student*)malloc(sizeof(student));
	student* s5=(student*)malloc(sizeof(student));
	strcpy(s1->name,"Peter");
	s1->cps = 20;
	s1->enrolled = IFM;
	
	strcpy(s2->name,"Caro");
	s2->cps = 25;
	s2->enrolled = IFM;
	
	strcpy(s3->name,"Jan");
	s3->cps = 0;
	s3->enrolled = PFLEGE;
	
	strcpy(s5->name,"Tim");
	s5->cps = 20;
	s5->enrolled = ELM;
	
	strcpy(s4->name,"Karl");
	s4->cps = 20;
	s4->enrolled = IFM;

	liste=append_student(NULL,s1);
	liste=append_student(liste,s2);
	liste=append_student(liste,s3);
	liste=append_student(liste,s4);
	liste=append_student(liste,s5);
	info=get_ifm_students(liste);

	printf("alle\n");
	show_all(liste);
	printf("\nAlle info\n");
	show_all(info);
	info=delete_node(info->next,NODE_AND_STUDENT);
	printf("\nAlle info\n");
	show_all(info);
	anzahl=delete_list_partial(info->next);
	show_all(info);
	printf("%d\n",anzahl);
	anzahl=delete_list(liste);
	printf("%d\n",anzahl);

	
	
	return 0;
}
