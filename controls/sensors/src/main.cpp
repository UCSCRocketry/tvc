#include "config.h"


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "sensors/imu.h"
#include "sensors/barometer.h"
#include "sensors/gps.h"


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


void setup(void)
{
  //serial initialization
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to initialize
  Serial.println("Connected to serial!");
  Serial.println("Initializing sensors...");
  // sensor setups
  bno.setup();
  bmp.setup();

}


void loop(void)
{


}
