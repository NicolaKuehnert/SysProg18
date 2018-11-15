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
    
    else
    {
		memblock* weiter=freemem;
		if((freemem->size)>=size){  						/*Reicht der erste Block bereits aus, wird freemem verschoben*/
			freemem=freemem->next;
			return weiter;
		}
		else{
			while(weiter->next!=NULL){						/*Durchlaufe die Freispeicherliste bis zum Ende*/
				if((weiter->next->size)>=size){				/*Size vom nächsten freien Block passt auf die Gesuchte Größe*/
					memblock* ret=weiter->next;
					ret->id=1; 			     				/*ID für den assoziierten Block*/
					weiter->next=weiter->next->next;
					ret=ret+sizeof(memblock);				/*Pointer auf den Nutzbaren Speicherberreich des gefundenen Freien Block*/
					ret->next=(memblock*)MAGIC_INT;					/*Belegten Speicherplatz verweist auf den Vorgegebenen Wert 0xacdcacdc*/
					return ret;
				}
			}
			return NULL;
		}
		
		
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
