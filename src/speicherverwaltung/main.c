#include <stdio.h>
#include "speicherverwaltung/speicherverwaltung.h"


int main(){
	memblock *ptr=NULL;
	memblock *ptr2=NULL;

	printf("Call malloc\n");
	ptr=(memblock *)cm_malloc(30);
	printf("%p\n\n",ptr);

	printf("Call malloc 2\n");
	ptr2=(memblock *)cm_malloc(100);
	printf("%p\n\n",ptr2);

	printf("Call free\n");
	cm_free(ptr);
	cm_free(ptr2);

	printf("%p\n",ptr);
	printf("%p\n\n",ptr2);

	printf("Call malloc 3\n");
	ptr=(memblock *)cm_malloc(30);
	printf("%p\n\n",ptr);

	printf("Call malloc 4\n");
	ptr2=(memblock *)cm_malloc(20);
	printf("%p\n\n",ptr2);

	printf("Call memcpy\n");
	cm_memcpy(ptr, ptr2, 20);
	printf("%p\n\n",ptr2);
	
	printf("Call free\n");
	cm_free(ptr);
	cm_free(ptr2);
	
	
	return 0;
}
