#include <stdio.h>
#include "speicherverwaltung/speicherverwaltung.h"


int main(){
	memblock *ptr=NULL;
	memblock *ptr2=NULL;
	memblock *ptr3=NULL;
	
	init_heap();
	printf("fremem %i\n\n", freemem);
	printf("\nCall malloc\n");
	ptr=(memblock *)cm_malloc(30);
	printf("\nCall malloc\n");
	cm_free(ptr);
	ptr2=(memblock *)cm_malloc(40);
	printf("\nCall malloc\n");

	ptr3=(memblock *)cm_malloc(50);
	/*printf("\nCall malloc 2\n");
	ptr2=(memblock *)cm_malloc(100);

	printf("\nCall malloc 3\n");
	ptr=(memblock *)cm_malloc(500);

	printf("\nCall malloc 4\n");
	ptr2=(memblock *)cm_malloc(20);

	/*printf("Call memcpy\n");
	cm_memcpy(ptr, ptr2, 20);
	printf("%p\n\n",ptr2);*/
	
	
	printf("\ndisplay\n");
	display_heap();
	
	
	return 0;
}
