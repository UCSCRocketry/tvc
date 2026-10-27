//main.cc file for PWM signal ESC motor. SAMPLE CODE 

const uint8_t ESC_PIN = 3;  //choosing pin 3 for the PWM signal

const uint32_t THROTTLE_MINIMUM = 1000; //0%, armed status
const uint32_t THROTTLE_MAX = 2000; //100%, indicator for full motor signal
const uint32_t PERIOD = 20000; //50 Hz, 20000 µs

inline uint32_t PERIOD_TO_DUTY(uint32_t period_us) {
    return (uint32_t)((period_us * 32767UL) / period_us);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    analogWriteFrequency(ESC_PIN, 50.0f); //50Hz frequency at 16 bits (but gets capped at 15)
    analogWriteResolution(16);

    analogWrite(ESC_PIN, PERIOD_TO_DUTY(THROTTLE_MINIMUM)); //arm ESC with minimum throttle
    Serial.println("ESC Armed. Ramping: ");
    delay(3000);
}

void loop() {
    for (uint32_t us = THROTTLE_MINIMUM, us <= ((0.70 * THROTTLE_MINIMUM) + THROTTLE_MINIMUM); us += 3) {
        analogWrite(ESC_PIN, PERIOD_TO_DUTY(us));
        delay(20);
    } //ramp up from minimum throttle to 70% of throttle (1700 us)

    Serial.println("70% throttle.");
    delay(3000);

    for (uint32_t us = ((0.70 * THROTTLE_MINIMUM) + THROTTLE_MINIMUM); us >= THROTTLE_MINIMUM; us -= 3) {
        analogWrite(ESC_PIN, PERIOD_TO_DUTY(us));
        delay(20);
    } //ramp back down to throttle minimum

    Serial.println("Cycle complete.");
    delay(2000);
}
