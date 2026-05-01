#include "sensors/gps.h"

GPS::GPS(HardwareSerial &gps_serial, int baud_rate) : gps(&gps_serial) {
    this->gps_ptr = &gps_serial;
    this->baud_rate = baud_rate;
}

void GPS::setup() {
    gps.begin(baud_rate);
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
}

void GPS::update() {
    gps.read();
    if (gps.newNMEAreceived()) {
        gps.parse(gps.lastNMEA());
    }
}

float GPS::get_latitude() {
    return gps.latitudeDegrees;
}

float GPS::get_longitude() {
    return gps.longitudeDegrees;
}

float GPS::get_altitude_meters() {
    return gps.altitude;
}

void GPS::set_origin(float latitude, float longitude, float altitude) {
    origin_lat = latitude;
    origin_lng = longitude;
    origin_alt = altitude;
}

// Placeholder for your math logic
float GPS::get_dist_origin_meter() { return 0.0; }
float GPS::get_bearing_origin_rad() { return 0.0; }

std::tuple<float, float, float> GPS::get_NED_from_origin() {
    return std::make_tuple(0.0f, 0.0f, 0.0f);
}