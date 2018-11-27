#include "ringbuffer/display.h"
#include "ringbuffer/ringbuffer.h"
#include <stdio.h>

int main(void) {
	ring_buffer* test = init_buffer(2, free );
	
	char data = 'a';
	char data2 = 'b';
	char data3 = 'c';
	
	// ich weiß nicht welches format data haben muss um es abzuspeichern
	write_buffer(test, &data);

	printf("Belegt: %i\n", test->count);
	printf("Groesse: %i\n", test->size);
	printf("Anfang: %i\n", test->head);
	
	char* data2 = (char*)malloc(sizeof("hallo"));
	data2 = "halla";
	char * p1 = & data;
	char * p2 = & data2;
	char * p3 = & data3;
	
	write_buffer(test, &data);
	write_buffer(test, &data2);

	printf("\ntest->elems 1: %p\ntest->head: %i\n", test->elems[1], test->head);
	printf("test->elems 2: %p\n", test->elems[2]);
	char *ret = (char*)read_buffer(test);
	printf("Readbuffer return value: %p\n", ret);
	printf("\ntest->head: %i\n", test->head);
	*ret = (char*)read_buffer(test);
	printf("Readbuffer return value: %p\n\n", ret);
	
	printf("Belegt: %i\n", test->count);
	printf("Groesse: %i\n", test->size);
	printf("Anfang: %i\n", test->head);
	
	char* data3 = (char*)malloc(sizeof("hallo"));
	data3 = "halli";
	
	
	void** blub = &data;
	printf("Test%x", (char*)(*blub));
	
	write_buffer(test, &data3);
	
	printf("Belegt: %i\n", test->count);
	printf("Groesse: %i\n", test->size);
	printf("Anfang: %i\n", test->head);

	
	write_buffer(test, &data3);
	
	//free_buffer(test);
	
	return 0;
}

void free (void* buffer){
	printf("FREE: %s\n", *(char*)buffer);
}

