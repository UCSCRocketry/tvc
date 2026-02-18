#include "config.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "sensors/imu.h"
#include "sensors/barometer.h"
#include "sensors/gps.h"
#include "motors/servo_drivers.h"


/*
List of sensors:
    - Adafruit BNO055 IMU (i2c)
    - BMP388 Barometric Pressure Sensor (i2c)
    - gy-gpsv3-neo 7m (uart 9600 baud by defualt)
    - ESP32 as transceiver (SPI)
    - Benewake TFMini-S LiDAR (
*/



BNO055_IMU bno(BNO055_I2C_ADDRESS, BNO055_WIRE); // use BNO055_ADDRESS_B if A doesn't work
BMP388_Barometer bmp(BMP388_I2C_ADDRESS, BMP388_WIRE);
GPS gps(GPS_SERIAL);


Servo_Axis servo_y(SERVO_PIN_Y);
Servo_Axis servo_z(SERVO_PIN_Z);
Gimbal gimbal(servo_y, servo_z);



void setup(void)
{
  //serial initialization
  Serial.begin(9600);
  delay(1000);
  while(!Serial){}

  Serial.println("Connected to serial!");
  Serial.println("Initializing sensors...");
  


  // sensor setups
  bno.setup();
  bmp.setup();

  // //servo wiggle
  // Serial.println("Wiggling servos...");
  // gimbal.drive_servos(0.0, 0.0);
  // delay(1000);
  // float deg_30 = 30.0 * DEG_TO_RAD;
  // gimbal.drive_servos(deg_30, deg_30);
  // delay(1000);
  // gimbal.drive_servos(-deg_30, deg_30);
  // delay(1000);
  // gimbal.drive_servos(-deg_30, -deg_30);
  // delay(1000);
  // gimbal.drive_servos(deg_30, -deg_30)
  // delay(1000);
  // gimbal.drive_servos(deg_30, deg_30);
  // delay(1000);
  
  

  


}

void loop(void)
{
  Vector3 accel = bno.getAccel();
  Vector3 gyro = bno.getGyro();
  Vector3 mag = bno.getMagnetometer();
  Vector3 linaccel = bno.getLinearAccel();
  Quaternion quat = bno.getQuaternion();
  float temperature = bmp.getTemperature();
  float pressure = bmp.getPressure();
  float altitude = bmp.getAltitude();

  Serial.print("Accel (m/s^2): X="); Serial.print(accel.x());
  Serial.print(" Y="); Serial.print(accel.y());
  Serial.print(" Z="); Serial.print(accel.z());
  Serial.print(" | Gyro (deg/s): X="); Serial.print(gyro.x());
  Serial.print(" Y="); Serial.print(gyro.y());
  Serial.print(" Z="); Serial.print(gyro.z());
  Serial.print(" | Quaternion: W="); Serial.print(quat.w());
  Serial.print(" altitude (m): "); Serial.print(altitude);
  Serial.println();
  


  delay(1000); //delay for readability
}
