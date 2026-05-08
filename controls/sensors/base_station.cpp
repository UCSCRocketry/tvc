#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001"; // Must match the rocket's address

struct TelemetryPacket {
    uint32_t timestamp;
    float altitude;
    float temperature;
    float pitch;
    float roll;
    float latitude;
    float longitude;
};

TelemetryPacket incomingData;

void rxInit() {
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setAutoAck(true);
    radio.openReadingPipe(1, address);
    radio.startListening();
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    rxInit();
    Serial.println("Ground station listening...");
}

void loop() {
    if (radio.available()) {
        radio.read(&incomingData, sizeof(incomingData));

        Serial.print("Time: "); Serial.print(incomingData.timestamp);
        Serial.print(" | Alt: "); Serial.print(incomingData.altitude);
        Serial.print(" | Temp: "); Serial.print(incomingData.temperature);
        Serial.print(" | Pitch: "); Serial.print(incomingData.pitch);
        Serial.print(" | Roll: "); Serial.print(incomingData.roll);
        Serial.print(" | Lat: "); Serial.print(incomingData.latitude, 6);
        Serial.print(" | Lon: "); Serial.println(incomingData.longitude, 6);
    }
}
