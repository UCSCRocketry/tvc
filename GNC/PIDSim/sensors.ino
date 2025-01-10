// Include necessary sensor libraries
#include <Wire.h>
#include <> // GPS
#include <MS5607 Arduino Library.zip>  //  altimeter sensor
#include <Adafruit_BNO0ff-master.zip>         //  IMU sensor

// Create sensor objects
// Initialize sensor variables
float altitude, attitude;
double angle_x, angle_y, angle_z;

void setup_sensors() {
  // Initialize altimeter sensor
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor");
    while (1);
  }

  // Initialize IMU sensor
  mpu.initialize();
}

void read_sensors() {
  // Read altitude from the altimeter sensor
  altitude = bme.readAltitude(1013.25);  // Sea level pressure (example)

  // Read attitude (angles) from the IMU sensor
  mpu.getRotation(&angle_x, &angle_y, &angle_z);
  attitude = angle_x;  // Use the x-axis angle for attitude control
}