#include <stdio.h>
#include "speicherverwaltung/speicherverwaltung.h"


int main(){
	memblock *ptr=NULL;
	memblock *ptr2=NULL;
	memblock *ptr3=NULL;
	
	init_heap();
	printf("\nCall malloc\n");
	ptr=(memblock *)cm_malloc(30);
	printf("\nCall malloc\n");
	ptr2=(memblock *)cm_malloc(40);
	printf("\nCall malloc\n");
	
	ptr3=(memblock *)cm_malloc(50);
	cm_free(ptr3);
	printf("\nCall malloc 2\n");
	ptr2=(memblock *)cm_malloc(100);

	printf("\nCall malloc 3\n");
	ptr=(memblock *)cm_malloc(500);

	printf("\nCall malloc 4\n");
	ptr2=(memblock *)cm_malloc(20);

	printf("Call memcpy\n");
	cm_memcpy(ptr, ptr2, 20);
	
	
	printf("\ndisplay\n");
	//display_heap();
	
	
	return 0;
}
