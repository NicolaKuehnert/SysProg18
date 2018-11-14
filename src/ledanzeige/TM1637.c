/*
 * ============================================================================
 * Author:      Tobias Muth
 * Datum:       18.09.2017
 *                           Definitions for TM1637 (Vorgabe)
 * ============================================================================
 */

#include "ledanzeige/TM1637.h"
#include "ledanzeige/TM1637_intern.h"
#include "ledanzeige/segmentanzeige.h"

#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>

/* Definition von Konstanten fuer die Segmentanzeige */
#define ADDR_AUTO     0x40
#define ADDR_FIXED    0x44
#define STARTADDR     0xc0
#define DOT_OFFSET    0x80
#define BRIGHT_OFFSET 0x88

/*
 * Zuordnung der LEDs eines Segments zu den Bits des gesendeten Bytes
 *
 *     -A-
 *  F |   | B
 *     -G-
 *  E |   | C
 *     -D-
 */
const byte NUMBER_TO_BYTE[] = {
/*            XGFEDCBA; X=0: Dot aus, X=1: Dot an */
0x3f, /* 0: 0b00111111 */
0x06, /* 1: 0b00000110 */
0x5b, /* 2: 0b01011011 */
0x4f, /* 3: 0b01001111 */
0x66, /* 4: 0b01100110 */
0x6d, /* 5: 0b01101101 */
0x7d, /* 6: 0b01111101 */
0x07, /* 7: 0b00000111 */
0x7f, /* 8: 0b01111111 */
0x6f, /* 9: 0b01101111 */
0x77, /* A: 0b01110111 */
0x7c, /* b: 0b01111100 */
0x39, /* C: 0b00111001 */
0x5e, /* d: 0b01011110 */
0x79, /* E: 0b01111001 */
0x71 /* F: 0b01110001 */
};

/* Vorwaerts-Deklaration (Funktionen nicht in Schnittstelle) */
static void TM1637_start(void);
static void TM1637_stop(void);

/* globale Variable fuer die Helligkeit */
static brightness bright = BRIGHT;

/*
 * Initialisierung von WiringPi und der Segmentanzeige.
 * Muss genau einmal vor der Benutzung der Anzeige aufgerufen werden.
 */
int TM1637_setup() {
    static int device_initialised = 0;

    if (!device_initialised) {
        /* Versuche WiringPi zu initialisieren */
        if (wiringPiSetup() == -1) {
            fprintf(stderr, "setup wiringPi failed ! n");
            return EXIT_FAILURE;
        }

        /* Jetzt noch die Segmentanzeige initialisieren */
        /* beide Pins als Output konfigurieren */
        pinMode(PIN_CLOCK, OUTPUT);
        pinMode(PIN_DATA, OUTPUT);

        /* Vorbereitung für start(): beide Pins auf 1 */
        digitalWrite(PIN_CLOCK, HIGH);
        digitalWrite(PIN_DATA, HIGH);
        delayMicroseconds(DELAY_TIMER);

        /* Anzeige löschen */
        TM1637_clear_display();

        /* Initialisierung nicht mehrfach durchfuehren*/
        device_initialised = 1;

        return EXIT_SUCCESS;
    }

    /* FAILURE falls bereits initialisiert */
    return EXIT_FAILURE;
}

/*
 * Low-Level Funktion zum Abschliessen des Datentransfers.
 * Muss am Ende von TM1637_write_byte() aufgerufen werden.
 */
void TM1637_ack() {
    int count = 0;

    /*
     * Starte ack():
     * CLK auf 0, DIO auf 1 (wird durch den Chip heruntergezogen)
     */
    digitalWrite(PIN_CLOCK, LOW);
    delayMicroseconds(DELAY_TIMER);

    /* DIO als Input konfigurieren (wir wollen die 0 vom runterziehen lesen) */
    digitalWrite(PIN_DATA, HIGH);
    pinMode(PIN_DATA, INPUT);
    delayMicroseconds(DELAY_TIMER);

    /* CLK auf 1: Warte auf das ACK vom Chip */
    digitalWrite(PIN_CLOCK, HIGH);
    delayMicroseconds(DELAY_TIMER);

    /* versuche die 0 auf dem DIO zu lesen */
    while (digitalRead(PIN_DATA)) {
        count += 1;
        if (count == 200) {
            break; /* wir geben auf */
        }
    }

    /* CLK und DIO auf 0: Ausgangspunkt fuer stop */
    digitalWrite(PIN_CLOCK, LOW);
    delayMicroseconds(DELAY_TIMER);

    /* DIO-Pin restaurieren */
    pinMode(PIN_DATA, OUTPUT);
    digitalWrite(PIN_DATA, LOW);
    delayMicroseconds(DELAY_TIMER);
}

/*
 * Low-Level Funktion zum Setzen der Helligkeit aller Segmente.
 */
void TM1637_brightness(brightness b) {
    bright = b;
}

/*
 * Low-Level Funktionen zum Loeschen aller (bzw. eines) Segmente(s).
 */
void TM1637_clear_display(void) {
    TM1637_clear_segment(SEG1);
    TM1637_clear_segment(SEG2);
    TM1637_clear_segment(SEG3);
    TM1637_clear_segment(SEG4);
}

void TM1637_clear_segment(segment seg) {
    TM1637_display(seg, OFF, OFF);
}

/*
 * Low-Level Funktionen zum Setzen eines Segments.
 * Der Parameter data ist eine Bitsequenz nach NUMBER_TO_BYTE, ohne den Dezimalpunkt.
 */
void TM1637_display(segment seg, byte data, dot point) {
    byte seg_data;

    seg_data = data;
    if (point == ON) {
        seg_data = seg_data + DOT_OFFSET;
    }

    /* CMD1: Modus setzen */
    TM1637_start();
    TM1637_write_byte(ADDR_FIXED);
    TM1637_stop();

    /* CMD2: Daten fuer Segment schicken */
    TM1637_start();
    TM1637_write_byte(seg | STARTADDR);
    TM1637_write_byte(seg_data);
    TM1637_stop();

    /* CMD3: Helligkeit setzen */
    TM1637_start();
    TM1637_write_byte(BRIGHT_OFFSET + bright);
    TM1637_stop();
}

/*
 * High-Level Funktionen zum Setzen eines Segments.
 * Der Parameter number ist eine Gleitkommazahl zwischen -99.9 und 999.9 und
 * wird auf der Segmentanzeige mit drei Vorkomma- und einer Nachkommstelle
 * angezeigt.
 */
void TM1637_display_number(float number) {
    TM1637_display(SEG1, TM1637_calculate_display(number, SEG1), OFF);
    TM1637_display(SEG2, TM1637_calculate_display(number, SEG2), OFF);
    TM1637_display(SEG3, TM1637_calculate_display(number, SEG3), ON);
    TM1637_display(SEG4, TM1637_calculate_display(number, SEG4), OFF);
}

/*
 * Hilfsfunktion: Berechnet zu number die Bitsequenz für die Stelle seq.
 * Wird in TM1637_display_number() genutzt.
 */
byte TM1637_calculate_display(float number, segment seg) {
    static int lastnumber = 0;
    static byte segmente[] = { 0x3f, 0x3f, 0x3f, 0x3f };
    unsigned char digit;
    int reminder;

    /* Range Check */
    if (number > 999.9f) {
        fprintf(stderr,
                "calculateDisplay does not accept values higher than 999.9 \n");
        return 0x00; /* Error value */
    }
    if (number < -99.9f) {
        fprintf(stderr,
                "calculateDisplay does not accept values lower than -99.9 \n");
        return 0x00; /* Error value */
    }

    reminder = (int) (number * 10.0);
    if (reminder != lastnumber) {
        /* Neue Zahl ... alles neu berechnen */
        lastnumber = reminder;

        /* Vorzeichenbehandlung */
        if (reminder >= 0) {
            /* calculate first segment and subtract value from the rest of the value to be displayed */
            digit = reminder / 1000;
            reminder = reminder - digit * 1000;
            segmente[0] = NUMBER_TO_BYTE[digit];
        } else {
            reminder = abs(reminder);

            /* first segment displays a "-" */
            segmente[0] = 0x40;
        }

        /* calculate second segment and subtract value from the rest of the value to be displayed */
        digit = reminder / 100;
        reminder = reminder - digit * 100;
        segmente[1] = NUMBER_TO_BYTE[digit];

        /* calculate third segment and subtract value from the rest of the value to be displayed */
        digit = reminder / 10;
        reminder = reminder - digit * 10;
        segmente[2] = NUMBER_TO_BYTE[digit];

        /* calculate fourth segment and subtract value from the rest of the value to be displayed */
        segmente[3] = NUMBER_TO_BYTE[reminder];
    }

    /* Rueckgabe */
    switch (seg) {
    case SEG1:
        return segmente[0];
    case SEG2:
        return segmente[1];
    case SEG3:
        return segmente[2];
    case SEG4:
        return segmente[3];
    default:
        return 0x00; /* Error value */
    }
}

/*
 * Interne Hilfsfunktion: Start des Datentransfers
 */
static void TM1637_start(void) {
    /* beide Pins muessen bei 1 starten
     *
     * Sollte nach init() bzw. nach stop() bereit der Fall sein,
     * aber sicherheitshalber noch einmal einstellen
     */
    digitalWrite(PIN_CLOCK, HIGH);
    digitalWrite(PIN_DATA, HIGH);
    delayMicroseconds(DELAY_TIMER);

    /* DIO muss auf 0 gehen => Start */
    digitalWrite(PIN_DATA, LOW);
    delayMicroseconds(DELAY_TIMER);

    /* CLK muss ebenfalls auf 0 gehen */
    digitalWrite(PIN_CLOCK, LOW);
    delayMicroseconds(DELAY_TIMER);
}

/*
 * Interne Hilfsfunktion: Ende des Datentransfers
 */
static void TM1637_stop(void) {
    /* beide Pins muessen bei 0 starten
     *
     * Sollte nach ack() bereits der Fall sein,
     * aber sicherheitshalber noch einmal einstellen
     */
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_DATA, LOW);
    delayMicroseconds(DELAY_TIMER);

    /* CLK muss auf 1 gehen */
    digitalWrite(PIN_CLOCK, HIGH);
    delayMicroseconds(DELAY_TIMER);

    /* DIO muss ebenfalls auf 1 gehen */
    digitalWrite(PIN_DATA, HIGH);
    delayMicroseconds(DELAY_TIMER);
}

