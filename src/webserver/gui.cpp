#include "webserver/gui.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "webserver/client.h"

WINDOW * win;
int player_id = 0;


int _server(){
	try{
		int pid = fork();
		std::cout << pid;
		//wenn Kindprozess, dann den Server starten
		if(pid==0){
			char * const envi[] = {NULL};
			char * const command[] = {"./server", NULL};
			std::cout << "in child";
			execl("gnome-terminal", (char *) 0);
			execve("/usr/Desktop/SysProg18/src/webserver/server", command, envi);
		}
		return pid;
	} catch (...) {
		end();
		std::cout << "kein Fork";
	}
	std::cout << "Server läuft?";
}

void draw_board(){
	clear();
	//zeige den Rand schonmal an
	wrefresh(win);
	
	char header[] = "Score:  : Your Highscore:  : Game Highscore: ";
	char footer[] = "Green Magenta Red Cyan ";

	// Setze den Cursor für den Header und schreibe ein Leerzeichen
	mvwaddch(win, 0, 3, ' '| COLOR_PAIR(2));
	
	// Schreibe alles, was im String Header steht
	for(unsigned int i = 0; i < sizeof(header)-1; i++){
		waddch(win, header[i] | COLOR_PAIR(2));
	}
	
	// Setze Cursor so, dass wir Yellow schreiben können
	mvwaddch(win, 0, COLS-12, ' '| COLOR_PAIR(2));
	waddch(win, 'Y' | COLOR_PAIR(7));
	waddch(win, 'e' | COLOR_PAIR(7));
	waddch(win, 'l' | COLOR_PAIR(7));
	waddch(win, 'l' | COLOR_PAIR(7));
	waddch(win, 'o' | COLOR_PAIR(7));
	waddch(win, 'w' | COLOR_PAIR(7));
	
	// Setze den Cursor auf die letzte, sichtbare Zeile (Lines-1 groß, also Lines-2 die letzte, sichtbare Line)
	mvwaddch(win, LINES-2, 3, ' ' | COLOR_PAIR(2));
	
	//Count gibt an, welche Farben wir nutzen
	int count = 3;
	
	//Schreibe den Footer
	for(unsigned int i = 0; i < sizeof(footer)-1; i++){
		// Nach einem Leerzeichen kommt eine neue Farbe
		if(footer[i] == ' '){
			waddch(win, footer[i] | COLOR_PAIR(2));
			count++;
		}
		else{
			waddch(win, footer[i] | COLOR_PAIR(count));
		}
	}
	
	mvwaddch(win, 2, 2, ACS_DARROW | COLOR_PAIR(count));
	
	// Lade das Fenster neu, sodass der Text angezeigt wird
	wrefresh(win);

}

int init(){
	std::cout << "init";
	// muss aufgerufen werden, bevor ncurses genutzt werden kann
	initscr();
	// kein Enter benötigt, Tastendrücken wird sofort weitergegeben
	cbreak();
	// tastendruck wird nicht ausgegeben
	noecho();

	// initialisiert die Farben
	start_color();

	
	// Diese Farbpaare brauchen wir später. Ein Paar besteht immer aus seiner ID, der Vordergrundfarbe (VG) und der Hintergrundfarbe (HG)
	init_pair(2, 7, 0); //unser Standard: VG weiß, HG schwarz
	init_pair(3, 2, 0); //VG grün, HG schwarz
	init_pair(4, 5, 0); //VG magenta, HG schwarz
	init_pair(5, 1, 0); //VG rot, HG schwarz
	init_pair(6, 6, 0); //VG cyan, HG schwarz
	init_pair(7, 3, 0); //VG gelb, HG schwarz
	init_pair(8, 0, 7);


	// es werden auch Sondertasten genutzt (benötigt für Pfeiltasten)
	keypad(stdscr, TRUE);
	
	// legt die Größe des Fensters fest
	win = newwin(LINES-1, COLS-1, 0, 0);
	
	// zeichnet den Rand; Zeichen 0 ist die Linie;
	//			rechts, links, oben, unten, oben links, oben rechts, unten links, unten rechts
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	//zeige den Rand schonmal an
	wrefresh(win);
	
	char play[] = "Play game";
	char start_server[] = "Start a server";
	char arrow[] = "->";

	mvwaddch(win, (LINES/2)-2, (COLS/2)-8, ' ' | COLOR_PAIR(2));
	waddch(win, arrow[0] | COLOR_PAIR(8));
	waddch(win, arrow[1] | COLOR_PAIR(8));
	waddch(win, ' ' | COLOR_PAIR(2));

	for(int i = 0;i<sizeof(play)-1;i++){
		waddch(win, play[i] | COLOR_PAIR(2));
	}
	mvwaddch(win, LINES/2, (COLS/2)-5, ' ' | COLOR_PAIR(2));

	for(int i = 0;i<sizeof(start_server)-1;i++){
		waddch(win, start_server[i] | COLOR_PAIR(2));
	}

	// Setze die Standardfarbe des Fensters
	wattron(win, COLOR_PAIR(1));
	
	
	wrefresh(win);
	std::cout << "Menü geladen";

	int pos = 0;
	bool running = true;
	while(running){
		int ch = wgetch(win);
		switch(ch){
			case 's':
				if(pos == 0){
					mvwaddch(win, ((LINES/2)-2), ((COLS/2)-8), ' ' | COLOR_PAIR(2));
					waddch(win, ' ' | COLOR_PAIR(2));
					waddch(win, ' ' | COLOR_PAIR(2));
					mvwaddch(win, LINES/2, (COLS/2)-8, ' ' | COLOR_PAIR(2));
					waddch(win, arrow[0] | COLOR_PAIR(8));
					waddch(win, arrow[1] | COLOR_PAIR(8));
					pos = 1;
				}
				break;
			case 'w':
				if(pos == 1){
					mvwaddch(win, LINES/2, (COLS/2)-8, ' ' | COLOR_PAIR(2));
					waddch(win, ' ' | COLOR_PAIR(2));
					waddch(win, ' ' | COLOR_PAIR(2));
					mvwaddch(win, (LINES/2)-2, (COLS/2)-8, ' ' | COLOR_PAIR(2));
					waddch(win, arrow[0] | COLOR_PAIR(8));
					waddch(win, arrow[1] | COLOR_PAIR(8));
					pos = 0;
				}
				break;
			case '\n':
				if(pos == 0){
					std::cout << "Menü ende";
					running = false;
				}
				else if(pos == 1){
					endwin();
					std::cout << "Versuche Server zu starten";
					int id = _server();
					std::cout << id;
					running = false;
				}
			break;
		}
	}
	try{
		mvwaddch(win, ((LINES/2)-2), ((COLS/2)-8), ' ' | COLOR_PAIR(2));
		for(int i = 0;i<sizeof(play)+2;i++){
			waddch(win, ' ' | COLOR_PAIR(2));
		}
		mvwaddch(win, LINES/2, (COLS/2)-8, ' ' | COLOR_PAIR(2));

		for(int i = 0;i<sizeof(start_server)+2;i++){
			waddch(win, ' ' | COLOR_PAIR(2));
		}

		init_client();
		draw_board();
		handle_method(get_key);
	} catch (...) {
		end();
	}
	
	return 0;
	
}

/// Muss immer am Ende aufgerufen werden!
int end(){
	endwin();
	return 0;
}

int move()
{
	/*int running = 1;
	
	while(running){
		int ch = wgetch(win);
		sleep(1);
		// Auswerten des Inputs
		switch(ch){
			// Spielfigur nach links drehen
			case 'a':
				send_move("l");
				break;
			// Spielfigur nach rechts drehen
			case 'd':
				send_move("r");
				break;
			// Spiel beenden, wichtig: Shift + e drücken!
			case 'E':
			running = 0;
				break;
			// Alle anderen Tasten sollen ignoriert werden
			default:
				send_move("f");
				break;
		}
		wrefresh(win);		
    }
	endwin();
	return 0;*/
}

int get_key()
{
	int ch = wgetch(win);
	sleep(1);
	// Auswerten des Inputs
	switch(ch){
		// Spielfigur nach links drehen
		case 'a':
			send_move("l");
			break;
		// Spielfigur nach rechts drehen
		case 'd':
			send_move("r");
			break;
		// Spiel beenden, wichtig: Shift + e drücken!
		case 'E':
			return 0;
			break;
		// Alle anderen Tasten sollen ignoriert werden
		default:
			send_move("f");
			break;
	}
	wrefresh(win);
	return 1;
}

void set_position(int x, int y, int direction) {
	if (direction == 0 || direction == 2) {
		mvwaddch(win, x, y, ACS_VLINE | COLOR_PAIR(7));
	} else if(direction == 1 || direction == 3) {
		mvwaddch(win, x, y, ACS_HLINE | COLOR_PAIR(7));	
	}
}

void set_player_id(int id)
{
	player_id = id;
}

void send_move(char *direction)
{
	send_to_server(direction);
	char * result = receive_from_server();
	
	// ergebnis muss noch ausgelesen werden.....
	// dann set_position(x, y, direction);
}

