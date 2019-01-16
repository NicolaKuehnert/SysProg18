CFLAGS = -c -std=c++14 -Wall -pedantic -g
LIB_PATH = ../../lib
LIBS = -L$(LIB_PATH) -lledanzeige -ltempSensor
OFLAGS = -pthread -lcrypt -lm -lrt -lwiringPi -lsqlite3 -lncurses $(LIBS) -Wl,-rpath=$(LIB_PATH)
PROJECT = webserver
INCLUDE_PATH = ../../include
INCLUDE = -I$(INCLUDE_PATH) -I.


GUI_C = gui.cpp
GUI_H = $(INCLUDE_PATH)/$(PROJECT)/gui.h

SERVER_C = server.cpp
SERVER_H = $(INCLUDE_PATH)/$(PROJECT)/server.h

CLIENT_C = client.cpp
CLIENT_H = $(INCLUDE_PATH)/$(PROJECT)/client.h

GAME_LOGIC_C = game_logic.cpp
GAME_LOGIC_H = $(INCLUDE_PATH)/$(PROJECT)/game_logic.h

SQLITE_C = ../temperatur/sqlite_demo.cpp
SQLITE_H = $(INCLUDE_PATH)/temperatur/sqlite_demo.h

HANDLER_C = signalhandler.cpp
HANDLER_H = $(INCLUDE_PATH)/$(PROJECT)/signalhandler.h

MAIN_C = main.cpp


all: $(PROJECT) server

$(PROJECT): tempSensor.so ledanzeige.so client.o gui.o main.o
	g++ -o $(PROJECT) main.o gui.o client.o $(OFLAGS)
	
main.o: $(MAIN_C) $(GUI_H) $(SERVER_H) $()
	g++ $(CFLAGS) $(INCLUDE) $(MAIN_C)
	
server: server.o game_logic.o sqlite_demo.o signalhandler.o
	g++ -o server server.o game_logic.o sqlite_demo.o signalhandler.o $(OFLAGS)

gui.o: $(GUI_C) $(GUI_H)
	g++ $(CFLAGS) $(INCLUDE) $(GUI_C)
	
game_logic.o: $(GAME_LOGIC_C) $(GAME_LOGIC_H)
	g++ $(CFLAGS) $(INCLUDE) $(GAME_LOGIC_C)
	
server.o: $(SERVER_C) $(SERVER_H) 
	g++ $(CFLAGS) $(INCLUDE) $(SERVER_C)

sqlite_demo.o: $(SQLITE_C) $(SQLITE_H) 
	g++ $(CFLAGS) $(INCLUDE) $(SQLITE_C)
	
client.o: $(CLIENT_C) $(CLIENT_H)
	g++ $(CFLAGS) $(INCLUDE) $(CLIENT_C)
	
signalhandler.o: $(HANDLER_C) $(HANDLER_H)
	g++ $(CFLAGS) $(INCLUDE) $(HANDLER_C)
	
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
