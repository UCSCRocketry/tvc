#ifndef gps_H
#define gps_H

#include <Arduino.h>
#include <Adafruit_GPS.h>

using tuple = std::tuple;

class GPS{
public:
    GPS(HardwareSerial &gps_serial, int baud_rate);
    void setup();
    void update();
    float get_latitude();
    float get_longitude();
    float get_altitude_meters();

    void set_origin(float latitude, float longitude, float altitude);
    
    float get_dist_origin_meter(); //in meters, returns distance to origin
    float get_bearing_origin_rad(); // in radians, returns bearing to origin

    tuple<float,float,float> get_NED_from_origin(); // in meters, returns NE vector from origin (N, E, D)
    


private:
    Adafruit_GPS gps;
    HardwareSerial *gps_serial;
    int baud_rate;

    float origin_lat;
    float origin_lng;
    float origin_alt;


};






#endif