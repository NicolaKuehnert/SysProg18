#include "ledanzeige/segmentanzeige.h"
#include "ledanzeige/segmentanzeige.c"
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "ringbuffer/display.h"
#include "ringbuffer/ringbuffer.h"
#include "ledanzeige/TM1637.h"
#include "ledanzeige/TM1637.c"
void sleep(int time);
void display_status(void) {
	printf("Hallo\n");
	
	ring_buffer* test = init_buffer(3, free );
	char data = 'a';
	char data2 = 'b';

	TM1637_setup();

	write_buffer(test, &data);

	TM1637_display_number(percent(test));
	sleep(1);

	write_buffer(test, &data2);
	TM1637_display_number(percent(test));
	sleep(1);
	write_buffer(test, &data2);
	TM1637_display_number(percent(test));
	sleep(5);
	TM1637_clear_display();
}
