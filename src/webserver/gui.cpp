#include <ncurses.h>
#include "webserver/gui.h"
#include <unistd.h>
#include <temperatur/tempSensor.h>
#include <ledanzeige/TM1637.h>
#include <ledanzeige/segmentanzeige.h>
#include <iostream>
#include <math.h>


/**
@param raw Roher Temperaturwert
@return float Die aktuelle Temperatur
*/
float calculateTemperature(int raw){
	float temp = ((float)raw * (3.3/1024) -0.5) * 100;
	return temp;
}

TempSensor sensor = TempSensor(calculateTemperature);

int init(){
	// muss aufgerufen werden, bevor ncurses genutzt werden kann
	initscr();
	
	TM1637_setup();
	
	
	// initialisiert die Farben
	start_color();
	// kein Enter benötigt, Tastendrücken wird sofort weitergegeben
	cbreak();
	// tastendruck wird nicht ausgegeben
	noecho();
	// es werden auch Sondertasten genutzt (benötigt für Pfeiltasten)
	keypad(stdscr, TRUE);
	
	// legt die Größe des Fensters fest
	win = newwin(LINES-1, COLS-1, 0, 0);
	
	// zeichnet den Rand; Zeichen 0 ist die Linie;
	//			rechts, links, oben, unten, oben links, oben rechts, unten links, unten rechts
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	//zeige den Rand schonmal an
	wrefresh(win);
	
	char header[] = "Score:  : Your Highscore:  : Game Highscore: ";
	char footer[] = "Green Magenta Red Cyan ";
	
	// Setze die Standardfarbe des Fensters
	wattron(win, COLOR_PAIR(1));
	
	// Diese Farbpaare brauchen wir später. Ein Paar besteht immer aus seiner ID, der Vordergrundfarbe (VG) und der Hintergrundfarbe (HG)
	init_pair(2, 7, 0); //unser Standard: VG weiß, HG schwarz
	init_pair(3, 2, 0); //VG grün, HG schwarz
	init_pair(4, 5, 0); //VG magenta, HG schwarz
	init_pair(5, 1, 0); //VG rot, HG schwarz
	init_pair(6, 6, 0); //VG cyan, HG schwarz
	init_pair(7, 3, 0); //VG gelb, HG schwarz
	
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
	return 0;
	
}

/// Muss immer am Ende aufgerufen werden!
int end(){
	endwin();
	return 0;
}

int main(){
	
	float geschwindigkeit = sensor.getTemp();
	
	init();
	int running = 1;
	
	int curr_x = 2;
	int curr_y = 2;
	int curr_face = 0; // 0 down, 1 left, 2 up, 3 right
	
	while(running){
		int ch = wgetch(win);
		sleep(1);
		// Auswerten des Inputs
		switch(ch){
			// Spielfigur nach links drehen
			case 'a':
				switch(curr_face){
					case 0:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(5));
						mvwaddch(win, curr_x, curr_y-1, 'R' | COLOR_PAIR(5));
						curr_y--;
						curr_face = 3;
						break;
					case 1:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(5));
						mvwaddch(win, curr_x-1, curr_y, 'U' | COLOR_PAIR(5));
						curr_x--;
						curr_face = 2;
						break;
					case 2:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(5));
						mvwaddch(win, curr_x, curr_y+1, 'L' | COLOR_PAIR(5));
						curr_y++;
						curr_face = 1;
						break;
					case 3:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(5));
						mvwaddch(win, curr_x+1, curr_y, 'D' | COLOR_PAIR(5));
						curr_x++;
						curr_face = 0;
						break;
					}
				break;
			// Spielfigur nach rechts drehen
			case 'd':
				if(curr_face == 0){
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(6));
						mvwaddch(win, curr_x, curr_y+1, 'L' | COLOR_PAIR(6));
						curr_y++;
						curr_face = 1;
				}else if(curr_face == 3){
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(6));
						mvwaddch(win, curr_x-1, curr_y, 'U' | COLOR_PAIR(6));
						curr_x--;
						curr_face = 2;
				}else if(curr_face == 2){
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(6));
						mvwaddch(win, curr_x, curr_y-1, 'R' | COLOR_PAIR(6));
						curr_y--;
						curr_face = 3;
				}else if(curr_face == 1){
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(6));
						mvwaddch(win, curr_x+1, curr_y, 'D' | COLOR_PAIR(6));
						curr_x++;
						curr_face = 0;
				}
				break;
			// Spiel beenden, wichtig: Shift + e drücken!
			case 'E':
			running = 0;
				break;
			// Alle anderen Tasten sollen ignoriert werden
			default:
				switch(curr_face){
					case 0:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(7));
						mvwaddch(win, curr_x+1, curr_y, 'D' | COLOR_PAIR(7));
						curr_x++;
						break;
					case 1:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(7));
						mvwaddch(win, curr_x, curr_y+1, 'L' | COLOR_PAIR(7));
						curr_y++;
						break;
					case 2:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(7));
						mvwaddch(win, curr_x-1, curr_y, 'U' | COLOR_PAIR(7));
						curr_x--;
						break;
					case 3:
						mvwaddch(win, curr_x, curr_y, ACS_VLINE | COLOR_PAIR(7));
						mvwaddch(win, curr_x, curr_y-1, 'R' | COLOR_PAIR(7));
						curr_y--;
						break;
					}
				break;
		}
		wrefresh(win);
		
		
    }
    
	endwin();
}
