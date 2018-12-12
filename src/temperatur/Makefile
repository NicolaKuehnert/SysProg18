CFLAGS = -c -Wall -pedantic -g
LIB_PATH = ../../lib
LIBS = -L$(LIB_PATH) -lledanzeige -lringbuffer
OFLAGS = -pthread -lcrypt -lm -lrt -lcurl -lwiringPi $(LIBS) -Wl,-rpath=$(LIB_PATH)
PROJECT = temperatur
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.

MAIN_C = demo.cpp

TEMPERATUR_C = tempSensor.cpp
TEMPERATUR_H = $(INCLUDE_PATH)/temperatur/tempSensor.h

DISPLAY_C = display.c
DISPLAY_H = $(INCLUDE_PATH)/$(PROJECT)/display.h

CALC_TEMP_C = calcTemp.cpp
CALC_TEMP_H = $(INCLUDE_PATH)/$(PROJECT)/calcTemp.h

SQLITE_C = sql_demo.cpp
SQLITE_H = $(INCLUDE_PATH)/$(PROJECT)/sql_demo.h

CURL_C = curl_api.cpp
CURL_H = $(INCLUDE_PATH)/$(PROJECT)/curl_api.h

BUFFER_C = ../ringbuffer/ringbuffer.c
BUFFER_H = $(INCLUDE_PATH)/ringbuffer/ringbuffer.h

TM1637_C = ../ledanzeige/TM1637.c
SEG_C = ../ledanzeige/segmentanzeige.c


all: ringbuffer ledanzeige $(PROJECT) 

$(PROJECT): curl_api.o demo.o tempSensor.o calcTemp.o
	g++ -o $(PROJECT) calcTemp.o curl_api.o tempSensor.o demo.o $(OFLAGS)
	
demo.o: $(MAIN_C)
	g++ $(CFLAGS) $(INCLUDE) $(MAIN_C)

tempSensor.o: $(TEMPERATUR_C) $(TEMPERATUR_H)
	g++ $(CFLAGS) $(INCLUDE) $(TEMPERATUR_C)
	
calcTemp.o: $(CALC_TEMP_C) $(CALC_TEMP_H)
	g++ $(CFLAGS) $(INCLUDE) $(CALC_TEMP_C)
	
sqlite.o: $(SQLITE_C) $(SQLITE_H)
	g++ $(CFLAGS) $(INCLUDE) $(SQLITE_C)
	
ringbuffer: 
	make -C ../ringbuffer install_ringbuffer
	
curl_api.o: $(CURL_C) $(CURL_H)
	g++ $(CFLAGS) $(INCLUDE) $(CURL_C)
	
	
ledanzeige: $(TM1637_C)
	make -C ../../ install_ledanzeige
	
run: all
	clear && ./$(PROJECT)

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT) || make -C ../ringbuffer/ clean
