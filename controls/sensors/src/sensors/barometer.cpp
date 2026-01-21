#include "sensors/barometer.h"

BMP388_Barometer::BMP388_Barometer(int i2cAddress, TwoWire *wire) : bmp(Adafruit_BMP3XX()) {
    this->i2cAddress = i2cAddress;
    this->wire = wire;
}

void BMP388_Barometer::setup(){
    Serial.println("Attempting BMP388 connection...");
    if (!bmp.begin_I2C(i2cAddress, wire)){
        /* There was a problem detecting the BMP388 ... check your connections */
        Serial.print("NO BMP388 DETECTED");
        while (1);
    }
    Serial.println("BMP388 connected... setting up...");

    // set up other functions as needed, e.g., oversampling, filter coeff, data rate
    // bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    // bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    // bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    // bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    delay(25); // Allow sensor to stabilize
    
    Serial.println("BMP388 Initialized");
}

float BMP388_Barometer::getTemperature() {
    if (!bmp.performReading()) {
        Serial.println("Failed to perform reading from BMP388");
        return NAN; // return Not-A-Number on failure
    }
    return bmp.temperature;
}

float BMP388_Barometer::getPressure() {
    if (!bmp.performReading()) {
        Serial.println("Failed to perform reading from BMP388");
        return NAN; // return Not-A-Number on failure
    }
    return bmp.pressure;
}

float BMP388_Barometer::getAltitude() {
    if (!bmp.performReading()) {
        Serial.println("Failed to perform reading from BMP388");
        return NAN; // return Not-A-Number on failure
    }
    return bmp.readAltitude(SEA_LEVEL_PRESSURE_HPA);
}


