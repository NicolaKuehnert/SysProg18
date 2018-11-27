#include "ringbuffer/ringbuffer.h"
#include <stdio.h>

ring_buffer *buffer = NULL; 


ring_buffer *init_buffer(const size_t n, void (*f)(void *p)) {
	if(n > 0){
		buffer = (ring_buffer* )malloc(sizeof(ring_buffer));
		buffer->elems = (void*)malloc(n* sizeof(void*)); // unsicher ob das der richtige datentyp dafür ist
		buffer->free_callback = f;
		buffer->head = 0;
		buffer->count = 0;
		buffer->size = n;
	}
	return buffer;
}


void write_buffer(ring_buffer *cb, void *data) {
	printf("\nWRITE %p\n", (char*) data);
	if(cb->size == cb->head) {
		cb->head = 0;
		cb->free_callback(cb->elems[cb->head]); 
	} else {
		cb->head++;
	}
	cb->elems[cb->head] = data;
	printf("Test %p\n", (char*) cb->elems[cb->head]);
	if(cb->count < cb->size){
		cb->count++;
	}
}

void *read_buffer(ring_buffer *cb){
	if(cb->count == 0){
		return NULL;
	}
	else{
		//printf("Read buffer\nFirst Element: %p\n", (char *)cb->elems[cb->head]);
		printf("\ncb->head: %i\n", cb->head);
		void *ret = cb->elems[cb->head];				//Erstes Element, das noch nicht gelesen wurde
		cb->head--;							//Setze Head auf das nächste Element
		cb->count--;							//Verkleinere count, damit der Buffer weiß, wie viele Elemente noch da sind
		return ret;
	}

}

int free_buffer(ring_buffer *cb){
	return 0;
}

int count_elements(const ring_buffer *cb){
	if(cb->count == 0){
		return -1;
	}
	else {
		return cb->count;
	}
}
