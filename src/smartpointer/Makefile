CFLAGS = -c -std=c++14 -Wall -pedantic -g
LIB_PATH = ../../lib
LIBS = -L$(LIB_PATH) -lledanzeige
OFLAGS = -pthread -lcrypt -lm -lrt -lwiringPi 
PROJECT = smartpointer
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.

MAIN_C = main.cpp

REFCOUNTER_C = RefCounter.cpp
REFCOUNTER_H = $(INCLUDE_PATH)/$(PROJECT)/RefCounter.h

SMARTPTRIMPL_H = $(INCLUDE_PATH)/$(PROJECT)/SmartPointerImpl.h


all: $(PROJECT) 

$(PROJECT): main.o RefCounter.o
	g++ -o $(PROJECT) main.o RefCounter.o $(OFLAGS)
	
main.o: $(MAIN_C) $(SMARTPTRIMPL_H)
	g++ $(CFLAGS) $(INCLUDE) $(MAIN_C)
	
RefCounter.o: $(REFCOUNTER_C) $(REFCOUNTER_H)
	g++ $(CFLAGS) $(INCLUDE) $(REFCOUNTER_C)
	
run: all
	clear && ./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT)
