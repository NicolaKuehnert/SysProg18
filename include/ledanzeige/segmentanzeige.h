#ifndef LEDANZEIGE_H
#define LEDANZEIGE_H

typedef unsigned char byte;

typedef enum _segment {SEG1, SEG2, SEG3, SEG4}segment;
typedef enum _dot {OFF, ON}dot;
typedef enum _brightness {DARK, MEDIUM, BRIGHT = 7}brightness;

#endif
/**
Funktion zum Ausgeben eines Bytes
@param wr_data Eine float Zahl die ein Byte groß ist
*/
void TM1637_write_byte(byte wr_data);
