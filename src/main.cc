//main.cc file for PWM signal ESC motor. SAMPLE CODE 
#include <Arduino.h>

const uint8_t ESC_PIN = 3;  //choosing pin 3 for the PWM signal

const uint32_t THROTTLE_MINIMUM = 1000; //0%, armed status
const uint32_t THROTTLE_MAX = 2000; //100%, indicator for full motor signal
const uint32_t THROTTLE_TARGET = 1700; //70%
const uint32_t PERIOD = 20000; //50 Hz, 20000 µs
const uint32_t MAX_PWM_VAL = 65535; //16-bit timer max value

inline uint32_t PERIOD_TO_DUTY(uint32_t period_us) {
    return (uint32_t)((period_us * MAX_PWM_VAL) / PERIOD);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    analogWriteFrequency(ESC_PIN, 50.0f); //50Hz frequency at 16 bits
    analogWriteResolution(16);

    analogWrite(ESC_PIN, PERIOD_TO_DUTY(THROTTLE_MINIMUM)); //arm ESC with minimum throttle
    Serial.println("ESC Armed. Ramping: ");
    delay(3000);
}   

void loop() {
    Serial.println("Ramping up to 70% throttle.");
    for (uint32_t us = THROTTLE_MINIMUM; us <= THROTTLE_TARGET; us += 3) {
        analogWrite(ESC_PIN, PERIOD_TO_DUTY(us));
        delay(20);
    } //ramp up from minimum throttle to 70% of throttle (1700 us)

    Serial.println("70% throttle.");
    delay(3000);

    Serial.println("Ramping down to idle.");

    for (uint32_t us = THROTTLE_TARGET; us >= THROTTLE_MINIMUM; us -= 3) {
        analogWrite(ESC_PIN, PERIOD_TO_DUTY(us));
        delay(20);
    } //ramp back down to throttle minimum

    analogWrite(ESC_PIN, PERIOD_TO_DUTY(THROTTLE_MINIMUM));

    Serial.println("Cycle complete.");
    delay(2000);
}