#include "ringbuffer/ringbuffer.h"

ring_buffer *buffer = NULL; 
ring_buffer *init_buffer(const size_t n, void (*f)(void *p)) {
	if(n > 0){
		buffer = (ring_buffer* )malloc(sizeof(ring_buffer));
		buffer->elems = (void*)malloc(n* sizeof(void*)); // unsicher ob das der richtige datentyp dafür ist
		buffer->free_callback = f;
		buffer->head ,buffer->count = 0;
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
