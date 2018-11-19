#include "speicherverwaltung/speicherverwaltung.h"

unsigned char mempool[MEM_POOL_SIZE];
memblock *freemem=NULL;

int cm_init(){
	if(freemem){
		return 0; /* freemem zeigt schon auf einen Wert in dem Mempool, also schon initialisiert */
	} 
	else if(freemem == NULL) {
		freemem = (memblock*) mempool;
		freemem->size = sizeof(mempool)-sizeof(memblock);  /*Nutzbarer Speicherberreich*/
		freemem->next = (memblock*)MAGIC_INT;
		freemem->id = 0;
		return 1; /* freemem wurde initialisiert */
	}
	
	
	return -1; /* Irgendwas ist schiefgelaufen */
}

void *cm_malloc(size_t size){
	
	cm_init();
	
	if(size == 0)
    	{
        	return NULL;
    	}
    
    	else
    	{	
		memblock *weiter=freemem;
    		memblock *New_FreeBlock = NULL;
		int blockSize = 0;

		while(weiter->next!=NULL){						/*Durchlaufe die Freispeicherliste bis zum Ende*/
			if((weiter->size) >= size){  						/*Reicht der Block aus, wird freemem verschoben. -> first-fit */

				New_FreeBlock = weiter->next;

				#ifdef MALLOCSPLIT
				blockSize = weiter->size;

				if((weiter->size) > size + (2 * sizeof(memblock)) +32){
					weiter->size = size;
					New_FreeBlock->size = blockSize - size;
					New_FreeBlock->id = 0;
				}
				#endif

				freemem=New_FreeBlock;
				weiter->id = 1;
				weiter->next=(memblock*)MAGIC_INT;			/*Belegten Speicherplatz verweist auf den Vorgegebenen Wert 0xacdcacdc*/
				return weiter;
			}
			else {
				weiter = weiter->next;
			}

			/* Brauchen wir das hier dann noch?

			if((weiter->next->size)>=size){	
				memblock* ret=weiter->next;
				ret->id=1;
				weiter->next=weiter->next->next;
				ret=ret+sizeof(memblock);
				ret->next=(memblock*)MAGIC_INT;
				return ret;
			} */
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


int main() {return 0;}
