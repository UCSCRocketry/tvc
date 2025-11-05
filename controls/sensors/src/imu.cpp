#include "imu.h"

IMU::IMU(uint8_t address)
: bno(-1, address, &Wire)
{
}

bool IMU::begin() {
    if (!bno.begin()) {
        return false;
    }

    delay(1000);
    bno.setExtCrystalUse(true);
    return true;
}

void IMU::update() {
    // Read values
    euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    quat = bno.getQuat();
    temperature = bno.getTemp();

    bno.getCalibration(&sysCal, &gyroCal, &accelCal, &magCal);
}

imu::Vector<3> IMU::getEuler() { return euler; }
imu::Vector<3> IMU::getGyro() { return gyro; }
imu::Vector<3> IMU::getAccel() { return accel; }
imu::Quaternion IMU::getQuat() { return quat; }
int8_t IMU::getTemp() { return temperature; }

void IMU::getCalibration(uint8_t &sys, uint8_t &gyro, uint8_t &accel, uint8_t &mag) {
    sys = sysCal;
    gyro = gyroCal;
    accel = accelCal;
    mag = magCal;
}
