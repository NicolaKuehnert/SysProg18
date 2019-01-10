CFLAGS = -c -std=c++14 -Wall -pedantic -g
LIB_PATH = ../../lib
LIBS = -L$(LIB_PATH) -lledanzeige -ltempSensor
OFLAGS = -pthread -lcrypt -lm -lrt -lwiringPi -lncurses $(LIBS) -Wl,-rpath=$(LIB_PATH)
PROJECT = webserver
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.


GUI_C = gui.cpp
GUI_H = $(INCLUDE_PATH)/$(PROJECT)/gui.h

SERVER_C = server.cpp
SERVER_H = $(INCLUDE_PATH)/$(PROJECT)/server.h

CLIENT_C = client.cpp
CLIENT_H = $(INCLUDE_PATH)/$(PROJECT)/client.h

MAIN_C = main.cpp


all: $(PROJECT) 

$(PROJECT): tempSensor.so ledanzeige.so client.o gui.o main.o
	g++ -o $(PROJECT) main.o gui.o client.o $(OFLAGS)
	
main.o: $(MAIN_C) $(GUI_H) $(SERVER_H) $()
	g++ $(CFLAGS) $(INCLUDE) $(MAIN_C)
	
server: server.o
	g++ -o server server.o $(OFLAGS)

gui.o: $(GUI_C) $(GUI_H)
	g++ $(CFLAGS) $(INCLUDE) $(GUI_C)
	
server.o: $(SERVER_C) $(SERVER_H)
	g++ $(CFLAGS) $(INCLUDE) $(SERVER_C)
	
client.o: $(CLIENT_C) $(CLIENT_H)
	g++ $(CFLAGS) $(INCLUDE) $(CLIENT_C)
	
tempSensor.so:
	make -C ../temperatur install_tempSensor

	
ledanzeige.so:
	make -C ../.. install_ledanzeige
	
run: all
	clear && ./$(PROJECT)
	
run-server: server
	clear && ./server

test: all
	make -C ../../tests/$(PROJECT)

clean: 
	rm *.o $(PROJECT) server
