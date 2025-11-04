#include <SoftwareSerial.h>
#include <SimpleKalmanFilter.h>
#include <TFMini.h>


const char RX_PIN = 10, TX_PIN = 11;
SoftwareSerial mySerial(RX_PIN, TX_PIN);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)
TFMini tfmini;
// SimpleKalmanFilter tfFilter(/* params */);

void setup() {
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(115200);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
     
  Serial.println ("Initializing...");

  // Step 2: Initialize the data rate for the SoftwareSerial port
  mySerial.begin(TFMINI_BAUDRATE);

  // Step 3: Initialize the TF Mini sensor
  tfmini.begin(&mySerial);    
}


void loop() {
  // Take one TF Mini distance measurement
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

  // Display the measurement
  Serial.print(dist);
  Serial.print(" cm      sigstr: ");
  Serial.println(strength);

  // Wait some short time before taking the next measurement
  delay(25);  
}