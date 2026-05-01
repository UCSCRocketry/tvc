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
    gps.sendCommand( PMTK_SET_NMEA_UPDATE_10HZ); // 10 Hz update rate
    Serial.println("send initializing commands");
}

void GPS::update(){
    char c = gps.read();
    if (c) {
      Serial.print(c); 
    }

    
    if (gps.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
        if (!gps.parse(gps.lastNMEA())){ // this also sets the newNMEAreceived() flag to false
          return; // we can fail to parse a sentence in which case we should just wait for another
        }
    }
 

}

float GPS::get_longitude(){
    return gps.longitudeDegrees;
}

float GPS::get_latitude(){
    return gps.latitudeDegrees;
}

