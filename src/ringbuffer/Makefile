CFLAGS = -c -Wall -pedantic -g
PROJECT = ringbuffer
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.

MAIN_C = main.c

RINGBUFFER_C = $(PROJECT).c
RINGBUFFER_H = $(INCLUDE_PATH)/$(PROJECT).h

DISPLAY_C = display.c
DISPLAY_H = $(INCLUDE_PATH)/display.h


all: $(PROJECT)

ringbuffer: main.o $(PROJECT).o display.o
	gcc -o $(PROJECT) $(PROJECT).o display.o main.o
	
main.o: $(MAIN_C)
	gcc $(CFLAGS) $(INCLUDE) $(MAIN_C)

ringbuffer.o: $(RINGBUFFER_C)
	gcc $(CFLAGS) $(INCLUDE) $(RINGBUFFER_C)
	
display.o: $(DISPLAY_C) 
	gcc $(CFLAGS) $(INCLUDE) $(DISPLAY_C)
	
run: clean all
	./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT)
