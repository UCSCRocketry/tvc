#ifndef IMU_H
#define IMU_H   

#include <Arduino.h>
#include "config.h"
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

using Vector3 = imu::Vector<3>;
using Quaternion = imu::Quaternion; 

class BNO055_IMU {
public:
    BNO055_IMU(int i2cAddress = BNO055_ADDRESS_A, TwoWire *wire = &Wire);
    
    void setup();

    imu::Quaternion getQuaternion();
    imu::Vector<3> getGyro();
    imu::Vector<3> getAccel();
    imu::Vector<3> getMagnetometer();
    imu::Vector<3> getLinearAccel();

private:
    Adafruit_BNO055 bno;
};

#endif