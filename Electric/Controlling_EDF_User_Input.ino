#include <Servo.h>

// Define the ESC signal pin
const int ESC_PIN = 9;
Servo esc;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Attach the ESC to the Arduino
  esc.attach(ESC_PIN);

  // Step 1: Send maximum throttle for calibration
  Serial.println("Calibrating ESC - Sending max throttle...");
  esc.writeMicroseconds(2000); // Maximum throttle
  delay(3000);                 // Wait for ESC to register max throttle

  // Step 2: Send minimum throttle to complete calibration
  Serial.println("Calibrating ESC - Sending min throttle...");
  esc.writeMicroseconds(1000); // Minimum throttle
  delay(3000);                 // Wait for ESC to register min throttle

  // ESC should now be armed
  Serial.println("ESC calibrated and armed. Ready for operation.");
}

void loop() {
  int throttle = 1000; 
  int duration = 10000;      // Total duration in milliseconds (10 seconds)
  if (Serial.available() > 0) {
    userInput = Serial.parseInt();
    throttle = userInput;
  }
  // Optional: Hold at max throttle for 2 seconds
  delay(2000);
  // // Example throttle control
  // int throttle = 1100; // Set throttle value (adjust as needed)
  esc.writeMicroseconds(throttle); // Send throttle signal to ESC
  delay(10); // Small delay for stability
}
