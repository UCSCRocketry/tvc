#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class IMU {
public:
    IMU(uint8_t address = 0x28);    // Constructor

    bool begin();                   // Initialize IMU
    void update();                  // Refresh data from sensor

    // Accessors
    imu::Vector<3> getEuler();      // degrees
    imu::Vector<3> getGyro();       // rad/s
    imu::Vector<3> getAccel();      // m/s^2
    imu::Quaternion getQuat();
    int8_t getTemp();
    void getCalibration(uint8_t &sys, uint8_t &gyro, uint8_t &accel, uint8_t &mag);

private:
    Adafruit_BNO055 bno;
    imu::Vector<3> euler, gyro, accel;
    imu::Quaternion quat;
    int8_t temperature;

    uint8_t sysCal, gyroCal, accelCal, magCal;
};

#endif
