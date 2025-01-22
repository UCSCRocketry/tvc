#include <Wire.h>
#include <Adafruit_BNO055.h>

// Create an instance of the BNO055
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial);

  Serial.println("BNO055 Test");

  // Initialize the BNO055
  if (!bno.begin()) {
    Serial.println("Error: BNO055 not detected. Check wiring or I2C address!");
    while (1);
  }

  // Set sensor to calibration mode and configure units
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 initialized successfully!");
}

void loop() {
  // Get and print the Euler angles (heading, roll, pitch)
  sensors_event_t event;
  bno.getEvent(&event);

  Serial.print("Heading: ");
  Serial.print(event.orientation.x);
  Serial.print(" Roll: ");
  Serial.print(event.orientation.y);
  Serial.print(" Pitch: ");
  Serial.println(event.orientation.z);

  delay(500); // Delay to make readings readable
}
