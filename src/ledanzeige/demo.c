#include "ledanzeige/segmentanzeige.h"
#include "TM1637.c"
#include <unistd.h>

int main(){
	TM1637_setup();
	
	TM1637_display_number(15.5);
	
	sleep(10);
	TM1637_display_number(-65);
	
	sleep(10);
	TM1637_display_number(189.6);
	
	sleep(10);
	TM1637_clear_display();

	return 0;
}

