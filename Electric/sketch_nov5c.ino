

/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 1, TXPin = 0;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

    void setup(){
        Serial.begin(9600);
        ss.begin(GPSBaud);
        }
   
    void loop(){
        Serial.print(ss.available());
  // This sketch displays information every time a new sentence is correctly encoded
        while (ss.available() > 0){
          
        gps.encode(ss.read());
        if (gps.location.isUpdated()){
              Serial.print("Latitude= "); 
              Serial.print(gps.location.lat(), 6);
              Serial.print(" Longitude= "); 
              Serial.println(gps.location.lng(), 6);
              }
              }
        }  

