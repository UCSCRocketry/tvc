#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); 
const byte address[6] = "00001"; // Must match the rocket's address

struct TelemetryPacket {
    uint32_t timestamp;
    float altitude;
    float velocity;
};

TelemetryPacket incomingData;

void setup() {
    Serial.begin(115200);
    radio.begin();
    radio.openReadingPipe(1, address);
    radio.setPALevel(RF24_PA_MAX);
    radio.startListening(); // Set as receiver
}

void loop() {
    if (radio.available()) {
        radio.read(&incomingData, sizeof(TelemetryPacket));
        
        // Print to Serial Monitor so you can see it on your laptop
        Serial.print("Time: "); Serial.print(incomingData.timestamp);
        Serial.print(" | Alt: "); Serial.println(incomingData.altitude);
    }
}