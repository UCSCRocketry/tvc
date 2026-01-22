#include "sensors/imu.h"

#define BNO055_I2C_DEFAULT_ADDRESS 0x28 // default i2c address for bno055


BNO055_IMU::BNO055_IMU(int i2cAddress, TwoWire *wire) : bno(Adafruit_BNO055(-1, i2cAddress, wire)) {}


void BNO055_IMU::setup(){
    
    Serial.println("Attempting BNO055 connection...");
    if (!bno.begin()){
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("NO BNO055 DETECTED");
        while (1);
    }
    Serial.println("BNO055 connected... setting up...");
    
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_CONFIG);
    
    delay(25); // Allow sensor to stabilize

    Serial.println("BNO055 Initialized");

}
  

imu::Quaternion BNO055_IMU::getQuaternion() {
    return bno.getQuat();
}

imu::Vector<3> BNO055_IMU::getGyro() {
    return bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
}

imu::Vector<3> BNO055_IMU::getAccel() {
    return bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
}

imu::Vector<3> BNO055_IMU::getMagnetometer() {
    return bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
}

imu::Vector<3> BNO055_IMU::getLinearAccel() {
    return bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
}

