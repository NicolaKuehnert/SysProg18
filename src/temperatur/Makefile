CFLAGS = -c -Wall -pedantic -g
OFLAGS = -lwiringPi -pthread -lcrypt -lm -lrt
PROJECT = temperatur
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I../ -I.

MAIN_C = demo.cpp

TEMPERATUR_C = tempSensor.cpp
TEMPERATUR_H = $(INCLUDE_PATH)/tempSensor.h

DISPLAY_C = display.c
DISPLAY_H = $(INCLUDE_PATH)/$(PROJECT)/display.h

CALC_TEMP_C = calcTemp.cpp
CALC_TEMP_h = $(INCLUDE_PATH)/calcTemp.h

SQLITE_C = sqlite_demo.cpp
SQLITE_h = $(INCLUDE_PATH)/sqlite_demo.h

TM1637_C = ../ledanzeige/TM1637.c
SEG_C = ../ledanzeige/segmentanzeige.c


all: $(PROJECT)

$(PROJECT): demo.o tempSensor.o calcTemp.o
	g++ -o $(PROJECT) calcTemp.o tempSensor.o demo.o  $(OFLAGS)
	
demo.o: $(MAIN_C) 
	g++ $(CFLAGS) $(INCLUDE) $(MAIN_C)

tempSensor.o: $(TEMPERATUR_C)
	g++ $(CFLAGS) $(INCLUDE) $(TEMPERATUR_C)
	
calcTemp.o: $(CALC_TEMP_C)
	g++ $(CFLAGS) $(INCLUDE) $(CALC_TEMP_C)
	
sqlite.o: $(SQLITE_C)
	g++ $(CFLAGS) $(INCLUDE) $(SQLITE_C)
	
#display.o: $(DISPLAY_C) $(TM1637_C) $(SEG_C)
#	g++ $(CFLAGS) $(INCLUDE) $(DISPLAY_C) $(TM1637_C)
	
run: all
	clear && ./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT)
