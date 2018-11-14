/*
 * ============================================================================
 *                       Internal definitions for TM1637 (Vorgabe)
 * ============================================================================
 */

#ifndef TM1637_INTERNAL_H
#define TM1637_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ledanzeige/segmentanzeige.h"

/* Delay in Mikrosekunden, passend fuer TM1637_write_byte() */
#define DELAY_TIMER 100

/* Definition der Pins, wo die Segmentanzeige angeschlossen ist */
#define PIN_CLOCK 16 /* Clock-Pin, auf dem Shield PIN 15 */
#define PIN_DATA 27  /* Daten-Pin, auf dem Shield PIN 16 */

/*
 * Low-Level Funktion zum Abschliessen des Datentransfers.
 * Muss am Ende von TM1637_write_byte() aufgerufen werden.
 */
void TM1637_ack(void);

/*
 * Aufgabe: Schreibe ein Byte bitweise heraus (Start LSB)
 */
void TM1637_write_byte(byte wr_data);

/*
 * Low-Level Funktionen zum Setzen eines Segments.
 * Der Parameter data ist eine Bitsequenz nach NUMBER_TO_BYTE, ohne den Dezimalpunkt.
 */
void TM1637_display(segment seg, byte data, dot point);

/*
 * Hilfsfunktion: Berechnet zu number die Bitsequenz für die Stelle seq.
 * Wird in TM1637_display_number() genutzt.
 */
byte TM1637_calculate_display(float number, segment seg);

#ifdef __cplusplus
}
#endif

#endif /* TM1637_INTERNAL_H */
