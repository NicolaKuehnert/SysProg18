#include "speicherverwaltung/speicherverwaltung.h"
#include <stdio.h>

char mempool[MEM_POOL_SIZE];
memblock *freemem=NULL;

int init_heap(){
	if(freemem){
		return 0; /* freemem zeigt schon auf einen Wert in dem Mempool, also schon initialisiert */
	} 
	else if(freemem == NULL) {
		freemem = (memblock*) mempool;
		freemem->size = sizeof(mempool);  /*Nutzbarer Speicherberreich*/
		freemem->next = NULL;
		freemem->id = 0;
		return 1; /* freemem wurde initialisiert */
	}
	
	
	return -1; /* Irgendwas ist schiefgelaufen */
}

void *cm_malloc(size_t size){
	
	init_heap();
	if(size == 0)
    	{
        	return NULL;
    	}
	
    	else
    	{	
		memblock *weiter=freemem;
    		memblock *New_FreeBlock;
		int blockSize = 0;

		while(weiter->next!=(memblock*)MAGIC_INT){						/*Durchlaufe die Freispeicherliste bis zum Ende*/
			printf("%d\n",weiter->size);
			printf("Sizeof Memblock: %d\n", sizeof(memblock));

			if((weiter->size) >= size){  						/*Reicht der Block aus, wird freemem verschoben. -> first-fit */

				New_FreeBlock = weiter;						/*Neuer Block hat den Anfang des ersten freien Blocks*/
				printf("New_FreeBlock: %p\n",&New_FreeBlock);

				#ifdef MALLOCSPLIT
				blockSize = weiter->size;

				if((weiter->size) > size + (2 * sizeof(memblock)) +32){
					printf("Splitting\n");
					New_FreeBlock->size = size;
					New_FreeBlock->id += 1;
					New_FreeBlock->next=(memblock*)MAGIC_INT;
				}
				#endif

				/*
				*Hier müsste jetzt freemem noch auf den nächsten freien Bereich verschoben werden
				*/
				printf("New_FreeBlock: %x\n",New_FreeBlock);
				return New_FreeBlock;
			}
			else {
				weiter = weiter->next;
			}

		}
			
		return NULL;
		
	}
	
}

void cm_free(void *ptr){
	
	memblock* help_ptr = (memblock*) ptr;
	
	if(ptr != NULL && help_ptr->next == (memblock*)MAGIC_INT)/*wenn ptr = NULL || ptr nicht durch malloc angelegt*/
    	{
        	if(freemem != NULL)     /*Freispeicherliste existiert*/
        	{
            		if(freemem->next != (memblock*)MAGIC_INT)
            		{
                		help_ptr->next = freemem; /*freien Block markieren -> Liste existent*/
            		}
            		else
                		return; /*Fehler in der Verlinkung*/
        		}
        	else /*freemem -Zeiger zeigt nicht auf den physikalisch ersten Block*/
        	{
            		help_ptr->next = NULL; /*freien Block markieren -> Liste nicht existent*/
        	}

        	freemem = help_ptr;/*Block vorn in Liste einhängen*/
    	}	

   	return;
	
}

void cm_defrag(void){

}

void cm_defrag20(void){

}

void *cm_memcpy(void *dest, const void *src, size_t n){
	return 0;

}

void *cm_realloc(void *ptr, size_t size){
	return 0;
}


