#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdlib.h>
#define percent(x) ((100 / x->size) * count_elements(x))

/**
Ringbuffer Datentyp
@param size Die Größe des Buffers
@param count Die Anzahl der belegten Blöcke im Buffer
@param head Der Anfang des ersten freien Blocks
@param elems Alle Daten im Buffer
@param free_callback Funktions callback auf eine free Funktion
*/
typedef struct ring_buffer {
	size_t size, count, head;
	void **elems;
	void (*free_callback)(void *p);
}ring_buffer;
#endif 

/**
Initialisiert den Buffer
Der Buffer darf nur eimal initialisiert werden

@param n Größe des Buffers
@param *f callback auf eine Free Funktion

@return Ein Pointer auf den Ringbuffer
*/
ring_buffer *init_buffer(const size_t n, void (*f)(void *p));

/**
Schreibt ein Datenblock in den Buffer

@param *cb Der Buffer, in den geschrieben werden soll
@param *data Beliebige Daten, die in den Buffer geschrieben werden
*/
void write_buffer(ring_buffer *cb, void *data);

/**
Nimmt das erste Element aus dem Buffer und gibt die Daten davon aus

@param *cb Der Buffer, aus dem gelesen werden soll
*/
void *read_buffer(ring_buffer *cb);

/**
Zum Freigeben eines Buffers

@param *cb Der Buffer, der freigegeben werden soll
*/
int free_buffer(ring_buffer *cb);

/**
Zählt alle Elemente im Buffer und gibt die Anzahl zurück

@param *cb Der Buffer, in dem gezählt werden soll

@return int Die Anzahl der Elemente im Buffer
*/
int count_elements(const ring_buffer *cb);
