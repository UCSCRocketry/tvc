#include "sensors/gps.h"
#include <Arduino.h>
#include <Adafruit_GPS.h>

GPS::GPS(HardwareSerial &gps_serial, int baud_rate): gps(Adafruit_GPS(&gps_serial)) {
    this->gps_serial = &gps_serial;
    this->baud_rate = baud_rate;
}

void GPS::setup(){
    Serial.println("Initializing GPS...");
    gps.begin(baud_rate);
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ); // 10 second update time
}

void GPS::update(){
    gps.read();
}

float GPS::get_latitude(){
    return gps;
}

float GPS::get_longitude(){
    return gps.longitudeDegrees * DEG_TO_RAD;
}



