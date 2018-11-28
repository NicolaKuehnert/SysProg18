#include "ringbuffer/ringbuffer.h"
#include <stdio.h>

ring_buffer *buffer = NULL; 


ring_buffer *init_buffer(const size_t n, void (*f)(void *p)) {
	if(f==NULL){
		return NULL;
	}
	else if(n > 0){
		buffer = (ring_buffer* )malloc(sizeof(ring_buffer));
		buffer->elems = (void*)malloc(n* sizeof(void)); // unsicher ob das der richtige datentyp dafür ist

		if(f == NULL){
			buffer->free_callback = free;
		} else {
			buffer->free_callback = f;
		}

		buffer->head = 0;
		buffer->count = 0;
		buffer->size = n;
		return buffer;
	}
	return NULL;
}


void write_buffer(ring_buffer *cb, void *data) {
	if(cb && data){
		printf("\nWrite\n");
		test(cb);
		if(cb->count < cb->size  ){
			cb->count++;
		} else {
			//freigeben
			printf("content %c\n", *(char*)cb->elems[cb->head]);
			//cb->free_callback((void*)cb->elems[cb->head]); kann noch nicht die callback methode aufrufen
		}
		toNext(cb);
		cb->elems[cb->head] = data;
	}
}

void test(ring_buffer * cb){
	printf("Size: %u\n", (int)cb->size);
	printf("count: %u\n", count_elements(cb));
	printf("Head: %u\n\n", cb->head);
}

void *read_buffer(ring_buffer *cb){
	if(cb == NULL){
		return NULL;
	}
	if(cb->count == 0){
		return NULL;
	}
	else{
		void *ret = cb->elems[cb->head];				//Erstes Element, das noch nicht gelesen wurde
		toPrev(cb);							//Setze Head auf das nächst ältere Element
		cb->count--;							//Verkleinere count, damit der Buffer weiß, wie viele Elemente noch da sind

		return ret;
	}

}

void toPrev(ring_buffer *cb){
	if(cb->head < 0){
        	cb->head--;
    	}else{
        	cb->head = 0;
    	}
}

void toNext(ring_buffer *cb){
	if(cb->head <(cb->size)-1){
        cb->head++;
    }else{
        cb->head = 0;
    }
}

int free_buffer(ring_buffer *cb){
	if (cb) {
        printf("\nfree all\n");
        size_t count = cb->count;
        for(int i =0;i<count;i++){
            //(cb->free_callback)(*(cb->elems[cb->head]));
            toNext(cb);
        }
        return count;
    }
}

int count_elements(const ring_buffer *cb){
	if(!cb || cb->count < 0){
		return -1;
	}
	else {
		return cb->count;
	}
}
