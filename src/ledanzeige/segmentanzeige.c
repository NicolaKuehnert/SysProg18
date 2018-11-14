#include "ledanzeige/TM1637_intern.h"
#include <wiringPi.h>
#include <stdio.h>

/*
*	XGFEDCBA; X=0: Dot aus, X=1: Dot an
*
* Zuordnung der LEDs eines Segments zu den Bits des gesendeten Bytes
*
*     -A-
*  F |   | B
*     -G-
*  E |   | C
*     -D-
*/

void TM1637_write_byte(byte wr_data)
{
	
	int i;
	for(i = 0; i <= 7;i++) {
		
	if((wr_data & (1<<i)) > 0){
/*Write 1 (HIGH) for acitvated*/
		digitalWrite(PIN_CLOCK, LOW);
		delayMicroseconds(DELAY_TIMER);

		digitalWrite(PIN_DATA, HIGH);
		delayMicroseconds(DELAY_TIMER);


		digitalWrite(PIN_CLOCK, HIGH);
		delayMicroseconds(DELAY_TIMER);


	} else {
/*Write 0 (LOW) for deactivated*/
		digitalWrite(PIN_CLOCK, LOW);
		delayMicroseconds(DELAY_TIMER);

		digitalWrite(PIN_DATA, LOW);
		delayMicroseconds(DELAY_TIMER);

		digitalWrite(PIN_CLOCK, HIGH);
		delayMicroseconds(DELAY_TIMER);
		}
		
	}
	TM1637_ack();
	
}

