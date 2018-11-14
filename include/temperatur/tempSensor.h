/*
 * tempSensor.h
 *
 */

#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

/* **************************** VORGABEN ********************************** */
class TempSensor {
public:
    TempSensor(float (*calcTempF)(int raw));
    float getTemp();

private:
    float (*fptr)(int raw);
    void tempSensorInitialize();
    static bool isInitialized;
};
/* **************************** VORGABEN ********************************** */

#endif /* TEMPSENSOR_H */

