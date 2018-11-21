#include <stdio.h>
#include "speicherverwaltung/speicherverwaltung.h"


int main(){
	memblock *ptr=NULL;
	memblock *ptr2=NULL;

	printf("\nCall malloc\n");
	ptr=(memblock *)cm_malloc(30);

	ptr2=(memblock *)cm_malloc(30);

	/*printf("\nCall malloc 2\n");
	ptr2=(memblock *)cm_malloc(100);

	printf("\nCall malloc 3\n");
	ptr=(memblock *)cm_malloc(500);

	printf("\nCall malloc 4\n");
	ptr2=(memblock *)cm_malloc(20);

	/*printf("Call memcpy\n");
	cm_memcpy(ptr, ptr2, 20);
	printf("%p\n\n",ptr2);*/
	
	printf("\nCall free\n");
	cm_free(ptr);
	cm_free(ptr2);
	printf("\nCall malloc\n");
	ptr=(memblock *)cm_malloc(30);

	ptr2=(memblock *)cm_malloc(40);

	printf("\nCall realloc\n");
	ptr2=(memblock *)cm_realloc(ptr2, 20);
	ptr2=(memblock *)cm_realloc(ptr2, 50);
	ptr2=(memblock *)cm_realloc(ptr2, 0);
	
	
	return 0;
}
