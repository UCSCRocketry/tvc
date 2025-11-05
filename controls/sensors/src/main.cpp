#include <Wire.h>
#include <Arduino.h>

#include "imu.h"


IMU imu; 

/* Set the delay between fresh samples */
#define SAMPLERATE_DELAY_MS (100)



// IMU INITIALIZATION (I2C)
// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                    id, address

// ############ PUT REST OF SENSOR INITALIZATION HERE ################




void setup(void){
  Serial.begin (115200);

  if(!imu.begin()){
    Serial.println("no imu detected");
    while(1) delay(1000);
  }
  Serial.println("IMU initialized");
}






void loop(void)
{
  imu.update();
  
  delay(SAMPLERATE_DELAY_MS);


}