CFLAGS = -c -Wall -pedantic -g
OFLAGS = -lwiringPi -pthread -lcrypt -lm -lrt
PROJECT = ringbuffer
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I../ -I.

MAIN_C = main.c

RINGBUFFER_C = $(PROJECT).c
RINGBUFFER_H = $(INCLUDE_PATH)/$(PROJECT).h

DISPLAY_C = display.c
DISPLAY_H = $(INCLUDE_PATH)/$(PROJECT)/display.h

TM1637_C = ../ledanzeige/TM1637.c
SEG_C = ../ledanzeige/segmentanzeige.c


all: $(PROJECT)

ringbuffer: main.o $(PROJECT).o display.o
	gcc -o $(PROJECT) $(PROJECT).o display.o main.o $(OFLAGS)
	
main.o: $(MAIN_C)
	gcc $(CFLAGS) $(INCLUDE) $(MAIN_C)

ringbuffer.o: $(RINGBUFFER_C)
	gcc $(CFLAGS) $(INCLUDE) $(RINGBUFFER_C)
	
display.o: $(DISPLAY_C) $(TM1637_C) $(SEG_C)
	gcc $(CFLAGS) $(INCLUDE) $(DISPLAY_C) $(TM1637_C)
	
libringbuffer.so: $(PROJECT).c
	gcc -c -fpic $(PROJECT).c $(INCLUDE)
	gcc -shared -o libringbuffer.so ringbuffer.o
	
install_buffer: libringbuffer.so
	mkdir -p ../../lib && mv libringbuffer.so ../../lib/libringbuffer.so
	rm ringbuffer.o
	
run: all
	clear
	./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT)
