#include "config.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
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

#define GPSECHO  true


BNO055_IMU bno(BNO055_I2C_ADDRESS, BNO055_WIRE); // use BNO055_ADDRESS_B if A doesn't work
BMP388_Barometer bmp(BMP388_I2C_ADDRESS, BMP388_WIRE);
GPS gps(GPS_SERIAL, 9600);


Servo_Axis servo_y(SERVO_PIN_Y);
//Servo_Axis servo_z(SERVO_PIN_Z);
//Gimbal gimbal(servo_y, servo_z);

Servo servo;

void setup(void)
{
  //serial initialization
  servo.attach(10);
  servo_y.attach();
  Serial.begin(9600);
  delay(1000);
  while(!Serial){}

  Serial.println("Connected to serial!");
  Serial.println("Initializing sensors...");
  
  servo.writeMicroseconds(100);

  // sensor setups
  bno.setup();
  bmp.setup();
  gps.setup();

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
  // gps.update();
  servo_y.set_axis_angle(PI);
  servo_y.drive_servo();
  delay(2000);
  servo_y.set_axis_angle(0);
  servo_y.drive_servo(); 
  delay(2000);

  if(false){

    Vector3 accel = bno.getAccel();
    Vector3 gyro = bno.getGyro();
    Vector3 mag = bno.getMagnetometer();
    Vector3 linaccel = bno.getLinearAccel();
    Vector3 euler = bno.getEuler();
    float temperature = bmp.getTemperature();
    float pressure = bmp.getPressure();
    float altitude = bmp.getAltitude();

    Serial.print("Accel (m/s^2): X="); Serial.print(accel.x());
    Serial.print(" Y="); Serial.print(accel.y());
    Serial.print(" Z="); Serial.print(accel.z());
    Serial.print(" Euler deg: X="); Serial.print(euler.x() );
    Serial.print(" Y="); Serial.print(euler.y() );
    Serial.print(" Z="); Serial.print(euler.z() );
    Serial.print(" altitude (m): "); Serial.print(altitude);
    Serial.print(" calibration status: "); Serial.print(bno.getCalibrationStatus());
    Serial.println();
  }
  


}
