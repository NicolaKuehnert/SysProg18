#include "ringbuffer/display.h"
#include "ringbuffer/ringbuffer.h"

int main(void) {
	ring_buffer* test = init_buffer(2, free );
	printf("Belegt: %i\n", test->count);
	printf("Groesse: %i\n", test->size);
	printf("Anfang: %i\n", test->head);
	
	char* data = (char*)malloc(sizeof("hallo"));
	data = "hallo";
	
	// ich weiß nicht welches format data haben muss um es abzuspeichern
	write_buffer(test, &data);
	
	printf("Belegt: %i\n", test->count);
	printf("Groesse: %i\n", test->size);
	printf("Anfang: %i\n", test->head);
	
	char* data2 = (char*)malloc(sizeof("hallo"));
	data2 = "halla";
	
	write_buffer(test, &data2);
	
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
	return 0;
}

void free (void* buffer){
	printf("FREE: %p\n", buffer);
}
