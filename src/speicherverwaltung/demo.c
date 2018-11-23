#include <stdio.h>
#include "../../include/speicherverwaltung/speicherverwaltung.h"


int main(){
	memblock *ptr=NULL;
	ptr=(memblock *)cm_malloc(5);
	printf("%d",&ptr);
	cm_free(ptr);
	
	return 0;
}
