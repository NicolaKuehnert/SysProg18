#include "speicherverwaltung/speicherverwaltung.h"
#include "ledanzeige/segmentanzeige.h"
#include "ledanzeige/TM1637.h"
#include "ledanzeige/TM1637_intern.h"
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "speicherverwaltung/display.h"

void display_heap(void) {
    int countFree = 0, countFilled = 0, memAllocated = 0;
    memblock *block = (memblock *) mempool;
    while (inRange(block) && block->next != 0 && block->size != 0) {
        if (block->next == toMemblockPtr(MAGIC_INT)) {
            countFilled++;
            memAllocated += sizeof(memblock) + block->size;
        } else {
            countFree++;
        }
        printf("%i => ", block);
        printf("%i next =>", block->size);
        printf("%d\n", block->next);
		block = nextBlock(block);
    }
    
    printf("belegt %i\n", countFilled);
    printf("frei %i\n", countFree);
    printf("size %i\n", memAllocated);
    /*TM1637_display_number((float) countFree);
    delayMicroseconds(WAIT);
    TM1637_display_number((float) countFilled);
    delayMicroseconds(WAIT);
    TM1637_display_number((((double) memAllocated) / ((double) MEM_POOL_SIZE) * 100.0));
    delayMicroseconds(WAIT);
    delayMicroseconds(WAIT);
    delayMicroseconds(WAIT);*/
}
