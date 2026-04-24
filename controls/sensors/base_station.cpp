#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include "datalog/transceiver.h"   // shared Telemetry struct + rxInit/processIncomingTelemetry

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
    Serial.begin(115200);
    rxInit();   
    Serial.println("Ground station listening...");
}

void loop() {
    processIncomingTelemetry();
}