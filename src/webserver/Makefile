CFLAGS = -c -std=c++14 -Wall -pedantic -g
LIB_PATH = ../../lib
LIBS = -L$(LIB_PATH) -lledanzeige
OFLAGS = -pthread -lcrypt -lm -lrt -lwiringPi 
PROJECT = webserver
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.


GUI_C = gui.cpp
GUI_h = $(INCLUDE_PATH)/$(PROJECT)/gui.h



all: $(PROJECT) 

$(PROJECT): gui.o
	g++ -o $(PROJECT) gui.o $(OFLAGS)
	
gui.o: $(GUI_C) $(GUI_H)
	g++ $(CFLAGS) $(INCLUDE) $(GUI_C)
	
run: all
	clear && ./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT)
