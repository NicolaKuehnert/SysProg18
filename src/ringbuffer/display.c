#include "ledanzeige/segmentanzeige.h"
#include "ledanzeige/segmentanzeige.c"
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "ringbuffer/display.h"
#include "ringbuffer/ringbuffer.h"
#include "ledanzeige/TM1637.c"

void display_status(void) {
	printf("Hallo\n");
	
	ring_buffer* test = init_buffer(2, free );
	char data = 'a';
	char data2 = 'b';
	char data3 = 'c';

	TM1637_setup();


	write_buffer(test, &data);

	TM1637_display_number(count_elements(test));
	sleep(1);

	write_buffer(test, &data2);
	write_buffer(test, &data2);

	TM1637_display_number(count_elements(test));
	
	sleep(2);
	
	TM1637_display_number(test->size);
	
	sleep(5);
	TM1637_clear_display();
}
