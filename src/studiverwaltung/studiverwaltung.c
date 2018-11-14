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
	Student* s1=(Student*)malloc(sizeof(Student));
	Student* s2=(Student*)malloc(sizeof(Student));
	Student* s3=(Student*)malloc(sizeof(Student));
	Student* s4=(Student*)malloc(sizeof(Student));
	Student* s5=(Student*)malloc(sizeof(Student));
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


node* get_ifm_students(node* n){
		if(n!=NULL){
			if(n->stdnt->enrolled==0){
				node * sp=(node*)malloc(sizeof(node));
				sp->stdnt=n->stdnt;
				sp->next=get_ifm_students(n->next);
				return sp;
			}
			else{
				return get_ifm_students(n->next);
			}
	}
	return NULL;
}

void show_all(node* n){
	if(n != NULL){
		printf("name: %s \ncps= %d \n\n",n->stdnt->name,n->stdnt->cps);
		n=n->next;
		show_all(n);
	}
	else{
		printf("Keine Weiteren Listen Einträge \n");
	}
		
}

node * append_student(node* n, Student* st){
	if(st!=NULL){
		if(n==NULL){
			node * sp=(node*)malloc(sizeof(node));
			sp->stdnt=st;
			sp->prev=NULL;
			sp->next=NULL;
			return sp;
		}
		else{
			if(n->next == NULL){
				node* sp=(node*)malloc(sizeof(node));
				sp->stdnt=st;
				n->next=sp;
				sp->prev=n;
				sp->next=NULL;
				while(n->prev !=NULL){
					n=n->prev;
				}
				return n;
			}
			else{
				n=n->next;
				return append_student(n,st);
			}		
		}
	}
	else{
		return NULL;
	}	
	
}

node* delete_node(node* nodetodel, sp_purge X){
		node*ret=NULL;
		if(nodetodel!=NULL){
			if(nodetodel->prev!=NULL){ /*Problem: hier geht er nicht rein, deshalb fehler bei erneutem Aufruf->Verbindung wird nicht gelöscht zu vorgänger*/
				printf("node prev_>next ändern \n");
				nodetodel->prev->next=nodetodel->next;
			}
			if(nodetodel->next!=NULL){
				nodetodel->next->prev=nodetodel->prev;
			}
		ret=nodetodel->next;
		if(ret==NULL){
			ret=nodetodel->prev;
		}
		if(X==NODE_AND_STUDENT){
			free(nodetodel->stdnt);
		}
		nodetodel->prev=NULL;
		nodetodel->next=NULL;
		nodetodel->stdnt=NULL;
		free(nodetodel);
		ret=getAnfang(ret);	
				
		return ret;
		}
		else{
			printf(" kein NODE vorhanden.\n");
			return NULL;
		}
		
		
	
}

int delete_list(node* n){
	int anz=0;
	n=getAnfang(n);
	printf("delete List\n");
	while(n!=NULL){
		node* b=delete_node(n,NODE_AND_STUDENT);
		anz++;
		n=b;
	}

	return anz;
	
}

int delete_list_partial(node* n){
	int anz=0;
	printf("delete List partial\n");
	while(n!=NULL){
		node* b=delete_node(n,NODE_ONLY);
		anz++;
		n=b;
	}
	return anz;

}

node* getAnfang(node* n){
	if(n==NULL){
		return NULL;
	}
	else{
		while(n->prev!=NULL){
			n=n->prev;
		}
		return n;
	}
}















