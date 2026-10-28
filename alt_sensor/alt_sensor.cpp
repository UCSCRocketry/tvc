#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP5xx.h>

Adafruit_BMP5xx bmp; //initialize sensor as bmp

void setup() {
  Serial.begin(115200); //start the serial monitor
  while (!Serial) { delay(10); } //wait for you to open it

  Serial.println("Initializing BMP585...");

  if (!bmp.begin(0x46, &Wire)) {  //0x46 is the default secret address for the sensor 
    Serial.println("Error: BMP585 not detected!");
    while (1) { delay(100); }  //freeze if sensor missing
  }

  //lwk dont know what below does 
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_32X); 
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ); //update 50 times per second
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);
}

void loop() {
  if (!bmp.performReading()) { return; }

  float alt = bmp.readAltitude(1013.25); //sea level 
  Serial.print("Altitude: "); //print info
  Serial.print(alt);
  Serial.println(" m");

  delay(250); 
}

