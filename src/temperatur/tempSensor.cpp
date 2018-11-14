/*
 * tempSensor.cpp
 *
 */

/* **************************** VORGABEN ********************************** */
#include <wiringPi.h>
#include <mcp3004.h> // 3004 und 3008 sind wohl beide mit diesem zu nutzen.

#include "temperatur/tempSensor.h"


// Anschluesse auf dem Pi
#define PIN_BASE 100
#define SPI_CHAN 0

// static Variable (Initialisierungsstatus)
bool TempSensor::isInitialized = false;


TempSensor::TempSensor(float (*calcTempF)(int raw)) :
        fptr(calcTempF) {
    tempSensorInitialize();
}

float TempSensor::getTemp() {
    // get the raw output from the temperature sensor and transform it
    return fptr(analogRead(PIN_BASE));
}

void TempSensor::tempSensorInitialize() {
    // initialize A/D with SPI channel and the PIN Base
    if (!isInitialized) {
        mcp3004Setup(PIN_BASE, SPI_CHAN);

        isInitialized = true;
    }
}
/* **************************** VORGABEN ********************************** */

