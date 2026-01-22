#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include "config.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

class BMP388_Barometer {
public:
    BMP388_Barometer(int i2cAddress = BMP3_ADDR_I2C_PRIM, TwoWire *wire = &Wire);
    
    void setup();

    float getTemperature();
    float getPressure();
    float getAltitude();

private:
    Adafruit_BMP3XX bmp;
    int i2cAddress;
    TwoWire *wire;
};

#endif