#include "ringbuffer/ringbuffer.h"
#include <stdio.h>

ring_buffer *buffer = NULL; 


ring_buffer *init_buffer(const size_t n, void (*f)(void *p)) {
	if(n > 0){
		buffer = (ring_buffer* )malloc(sizeof(ring_buffer));
		buffer->elems = (void*)malloc(n* sizeof(void)); // unsicher ob das der richtige datentyp dafür ist
		buffer->free_callback = f;
		buffer->head = 0;
		buffer->count = 0;
		buffer->size = n;
	}
	return buffer;
}


void write_buffer(ring_buffer *cb, void *data) {
	if(cb){
		printf("\nWrite\n");
		test(cb);
		if(cb->count < cb->size  ){
			cb->count++;
		} else {
			//freigeben
			printf("content %c\n", *(char*)cb->elems[cb->head]);
			//cb->free_callback((void*)cb->elems[cb->head]); kann noch nicht die callback methode aufrufen
		}
		cb->elems[cb->head] = data;
		toNext(cb);
	}
}

void test(ring_buffer * cb){
	printf("Size: %i\n", (int)cb->size);
	printf("count: %i\n", count_elements(cb));
	printf("Head: %i\n", cb->head);
}

void *read_buffer(ring_buffer *cb){
	if(cb->count == 0){
		return NULL;
	}
	else{
<<<<<<< HEAD
		//printf("Read buffer\nFirst Element: %p\n", (char *)cb->elems[cb->head]);
		printf("\ncb->head: %i\n", cb->head);
=======
		printf("First Element: %p\n", (char*)cb->elems[cb->head]);
>>>>>>> 98899c0c020220ed1c8854a91b25f7d0eb827cad
		void *ret = cb->elems[cb->head];				//Erstes Element, das noch nicht gelesen wurde
		cb->head--;							//Setze Head auf das nächste Element
		cb->count--;							//Verkleinere count, damit der Buffer weiß, wie viele Elemente noch da sind
		return ret;
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
