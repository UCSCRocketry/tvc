#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3);  // RX, TX pins (GPS TX -> D4)

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600); // GPS baud rate
  Serial.println("Starting GPS test");
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    Serial.print("Lat: "); Serial.print(gps.location.lat(), 6);
    Serial.print("  Lon: "); Serial.print(gps.location.lng(), 6);
    Serial.print("  Alt: "); Serial.print(gps.altitude.meters());
    Serial.print("  Sats: "); Serial.println(gps.satellites.value());
  }
}