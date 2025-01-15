#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_MS5607.h>
#include <Adafruit_GPS.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize sensors and peripherals
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MS5607 ms5607;
Adafruit_GPS GPS(&Serial1);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo configuration
#define SERVO_MIN 150
#define SERVO_MAX 600
#define SERVO_NEUTRAL ((SERVO_MIN + SERVO_MAX) / 2)

// PID constants
float Kp = 1.0;
float Ki = 0.1;
float Kd = 0.05;

// PID errors
float previousErrorPitch = 0;
float previousErrorYaw = 0;
float integralPitch = 0;
float integralYaw = 0;

void setup() {
  Serial.begin(115200);

  // Initialize sensors
  if (!bno.begin()) {
    Serial.println("Failed to initialize BNO055!");
    while (1);
  }
  if (!ms5607.begin()) {
    Serial.println("Failed to initialize MS5607!");
    while (1);
  }

  // Initialize GPS
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50);
}

int mapAngleToPulse(float angle) {
  return SERVO_NEUTRAL + (angle / 90.0) * (SERVO_MAX - SERVO_NEUTRAL);
}

void controlServos(float pitch, float yaw) {
  pwm.setPWM(0, 0, mapAngleToPulse(pitch));
  pwm.setPWM(1, 0, mapAngleToPulse(yaw));
}

void pidControl(float targetPitch, float targetYaw, float currentPitch, float currentYaw) {
  // Calculate errors
  float errorPitch = targetPitch - currentPitch;
  float errorYaw = targetYaw - currentYaw;

  // Update integrals
  integralPitch += errorPitch;
  integralYaw += errorYaw;

  // Calculate derivatives
  float derivativePitch = errorPitch - previousErrorPitch;
  float derivativeYaw = errorYaw - previousErrorYaw;

  // Compute control outputs
  float controlPitch = Kp * errorPitch + Ki * integralPitch + Kd * derivativePitch;
  float controlYaw = Kp * errorYaw + Ki * integralYaw + Kd * derivativeYaw;

  // Update servos
  controlServos(controlPitch, controlYaw);

  // Save previous errors
  previousErrorPitch = errorPitch;
  previousErrorYaw = errorYaw;
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  float currentPitch = event.orientation.x;
  float currentYaw = event.orientation.z;

  // Target orientation
  float targetPitch = 0.0;
  float targetYaw = 0.0;

  // Apply PID control
  pidControl(targetPitch, targetYaw, currentPitch, currentYaw);

  // Debugging output
  Serial.print("Current Pitch: ");
  Serial.print(currentPitch);
  Serial.print(" Current Yaw: ");
  Serial.println(currentYaw);

  delay(100);
}
