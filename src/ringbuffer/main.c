#include "ringbuffer/display.h"
#include "ringbuffer/ringbuffer.h"

int main(void) {
	ring_buffer* test = init_buffer(2, free );
	
	char data = 'a';
	char data2 = 'b';
	char data3 = 'c';
	
	char * p1 = & data;
	char * p2 = & data2;
	char * p3 = & data3;
	
	write_buffer(test, &data);
	write_buffer(test, &data2);
	write_buffer(test, &data3);
	
	//free_buffer(test);
	
	return 0;
}

void free (void* buffer){
	printf("FREE: %s\n", *(char*)buffer);
}

