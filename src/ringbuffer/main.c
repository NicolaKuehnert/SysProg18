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

	printf("Belegt: %li\n", test->count);
	printf("Groesse: %li\n", test->size);
	printf("Anfang: %li\n", test->head);
	
	
	write_buffer(test, &data);
	write_buffer(test, &data2);

	void *ret = read_buffer(test);
	printf("Readbuffer return value: %p\n\n", ret);

	/*printf("Belegt: %li\n", test->count);
	printf("Groesse: %li\n", test->size);
	printf("Anfang: %li\n", test->head);*/
	
	void** blub = &data;
	printf("Test%x", (char*)(*blub));
	
	write_buffer(test, &data3);

	ret = read_buffer(test);
	printf("Readbuffer return value: %p\n\n", ret);

	ret = read_buffer(NULL);
	printf("Readbuffer return value: %p\n\n", ret);

	ret = read_buffer(test);
	printf("Readbuffer return value: %p\n\n", ret);
	
	/*printf("Belegt: %li\n", test->count);
	printf("Groesse: %li\n", test->size);
	printf("Anfang: %li\n", test->head);*/

	
	write_buffer(test, &data3);

	printf("\ntest->head: %li\n", test->head);

	
	//free_buffer(test);
	
	return 0;
}

void free (void* buffer){
	printf("FREE: %s\n", *(char*)buffer);
}

