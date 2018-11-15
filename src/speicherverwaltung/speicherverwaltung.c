#include "speicherverwaltung/speicherverwaltung.h"

unsigned char mempool[MEM_POOL_SIZE];

int cm_init(){
	if(freemem){
		return 0; /* freemem zeigt schon auf einen Wert in dem Mempool, also schon initialisiert */
	} 
	else if(freemem == NULL) {
		freemem = (memblock*) mempool;
		freemem->size = sizeof(mempool);
		freemem->next = NULL;
		freemem->id = 0;
		return 1; /* freemem wurde inititalisiert */
	}
	
	return -1; /* Irgendwas ist schiefgelaufen */
}

void *cm_malloc(size_t size){
	
	cm_init();
	
	if(size == 0)
    {
        return NULL;
    }
	
}