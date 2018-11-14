/*
 * ============================================================================
 * Author:      Tobias Muth
 * Datum:       18.09.2017
 *                       Public definitions for TM1637 (Vorgabe)
 * ============================================================================
 */

#ifndef TM1637_H
#define TM1637_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ledanzeige/segmentanzeige.h"

/*
 * Initialisierung von WiringPi und der Segmentanzeige.
 * Muss genau einmal vor der Benutzung der Anzeige aufgerufen werden.
 */
int TM1637_setup(void);

/*
 * Low-Level Funktion zum Setzen der Helligkeit aller Segmente.
 */
void TM1637_brightness(brightness bright);

/*
 * Low-Level Funktionen zum Loeschen aller (bzw. eines) Segmente(s).
 */
void TM1637_clear_display(void);
void TM1637_clear_segment(segment seg);

/*
 * High-Level Funktionen zum Setzen eines Segments.
 * Der Parameter number ist eine Gleitkommazahl zwischen -99.9 und 999.9 und
 * wird auf der Segmentanzeige mit drei Vorkomma- und einer Nachkommstelle
 * angezeigt.
 */
void TM1637_display_number(float number);

#ifdef __cplusplus
}
#endif

#endif /* TM1637_H */
