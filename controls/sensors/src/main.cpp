#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


/* Set the delay between fresh samples */
#define SAMPLERATE_DELAY_MS (100)



// IMU INITIALIZATION (I2C)
// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                    id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1,  0x28, &Wire);

// ############ PUT REST OF SENSOR INITALIZATION HERE ################




void setup(void)
{
  //serial initialization
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to initialize


  //IMU initialization
  //initialize 
  if(!bno.begin())
  {
    Serial.print("BNO Not detected. quitting");
    while(1);
  }
  
  //IMU integrated temp resadings 
  int8_t temp = bno.getTemp();
  delay(1000);
  bno.setExtCrystalUse(true); //note: board may not include external crystal. External crystal for timing is better 




  //############ PUT REST OF SENSOR INTIALIZATION HERE



  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}

void loop(void)
{

  // IMU READING
  // Retrieves all data from IMU chip in two different formats:

  //in the adafruit unified sensor format.
  //declare sensor events
  sensors_event_t orientationData, angVelocityData, linearAccelData;
  sensors_event_t magnetometerData, accelerometerData, gravityData;

  // Get all the available sensor events
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);                    // degrees
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);                // rad/s
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);              // m/s^2
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);            // µT
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);          // m/s^2
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);                      // m/s^2

  //in vector3 form using imumaths lib
  // --- Read vector data (3D) ---
  imu::Vector<3> euler     = bno.getVector(Adafruit_BNO055::VECTOR_EULER);          // degrees
  imu::Vector<3> gyro      = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);      // rad/s
  imu::Vector<3> accel     = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);  // m/s^2
  imu::Vector<3> linAccel  = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);    // m/s^2
  imu::Vector<3> mag       = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);   // µT
  imu::Vector<3> gravity   = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);        // m/s^2

  // Quaternion -- same as vector3 euler or sensor_event orientationData, just in quaternion form instead
  imu::Quaternion quat = bno.getQuat();

  // Temperature (internal temperature of onbaord die, in celsius. shouldnt be more than 40 deg probably. check datasheet)
  int8_t temp = bno.getTemp();

  // Calibration status 
  // 0 is uncalibrated, 3 is fully callibrated. gets more calibrated when you move it around more. 
  uint8_t systemCal, gyroCal, accelCal, magCal = 0;
  bno.getCalibration(&systemCal, &gyroCal, &accelCal, &magCal);

  // ################# PUT REST OF SENSOR LOOP STUFF HERE ###################

  
  delay(SAMPLERATE_DELAY_MS);


}