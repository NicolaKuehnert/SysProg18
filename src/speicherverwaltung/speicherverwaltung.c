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

			if((weiter->size) >= size){  						/*Reicht der Block aus, wird freemem verschoben. -> first-fit */

				New_FreeBlock = weiter;						/*Neuer Block hat den Anfang des ersten freien Blocks*/

				#ifdef MALLOCSPLIT
				blockSize = weiter->size;

				if((weiter->size) > size + (2 * sizeof(memblock)) +32){
					printf("Splitting\n");
					New_FreeBlock->size = size;
					New_FreeBlock->id += 1;
					New_FreeBlock->next=(memblock*)MAGIC_INT;
				}
				#endif
				printf("New_FreeBlock: %x\n",New_FreeBlock);

				freemem = (memblock *)(((char * )freemem + 1) + size);
				freemem->size = blockSize - size;
				printf("freemem: %x\n", freemem);

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
	printf("freemem: %x\n", freemem);
	memblock* help_ptr = (memblock*) ptr;
	
	if(ptr != NULL && help_ptr->next == (memblock*)MAGIC_INT)/*wenn ptr = NULL || ptr nicht durch malloc angelegt*/
    {
		if(inRange(help_ptr)){
        	help_ptr->next = freemem;
        	freemem = help_ptr;
		}
    }	
	printf("freemem: %x\n", freemem);
   	return;
	
}

void cm_defrag(void){

}

void cm_defrag20(void){

}

//will noch nicht ganz, die pointer geben nil aus. sollte aber funktionieren

void *cm_memcpy(void *dest, const void *src, size_t n){
	printf("%p\n\n",toMemblockPtr(src));
	printf("%p\n\n",src);
	if (dest && src && inRange(dest) && inRange(src)) {
		printf("test");
        if (((toMemblockPtr(src) - 1)->next) == toMemblockPtr(MAGIC_INT) && ((toMemblockPtr(dest) - 1)->next) == toMemblockPtr(MAGIC_INT)) {
			printf("test2");
            char *destnew = ((char *) dest);
            char *srcnew = ((char *) src);
            for (int i = 0; (size_t) i < n; i++, destnew++, srcnew++) {
                *destnew = *srcnew;
            }
        }
        //erster Problemfall: dest nicht gross genug
        //zweiter Problemfall: src enthaelt nicht n elemente
        //dritter Problemfall: wenn n negativ ist
    }

}

void *cm_realloc(void *ptr, size_t size){
	return 0;
}


