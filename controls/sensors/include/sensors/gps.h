#ifndef gps_H
#define gps_H

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <tuple> // Required for std::tuple

class GPS {
public:
    // Pass the serial port by pointer or reference; here we use reference to match your logic
    GPS(HardwareSerial &gps_serial, int baud_rate);
    void setup();
    void update();
    float get_latitude();
    float get_longitude();
    float get_altitude_meters();

    void set_origin(float latitude, float longitude, float altitude);
    
    float get_dist_origin_meter(); 
    float get_bearing_origin_rad(); 

    // Explicitly define the types for the tuple
    std::tuple<float, float, float> get_NED_from_origin(); 

private:
    Adafruit_GPS gps;
    HardwareSerial *gps_ptr; // Store the pointer to the serial port
    int baud_rate;

    float origin_lat;
    float origin_lng;
    float origin_alt;
};

#endif