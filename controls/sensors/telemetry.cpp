#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <SD.h>

// NRF24L01+ Pin Setup for Teensy 4.1
RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001";

// structure to organize data for transmission
struct TelemetryPacket {
    uint32_t timestamp;
    float altitude;
    float velocity;
};

TelemetryPacket data;

void setup() {
    Serial.begin(115200);

    // initialize built-in SD card on Teensy 4.1
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD initialization failed!");
    }

    // Initialize Radio
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX); // Maximum power for range
    radio.stopListening();         // Set as transmitter
}

void loop() {
    // fill data structure (replace with actual sensor reads later)
    data.timestamp = millis();
    data.altitude = 150.0; 

    // 1. log to CSV file
    File logger = SD.open("flight_data.csv", FILE_WRITE);
    if (logger) {
        logger.print(data.timestamp);
        logger.print(",");
        logger.println(data.altitude);
        logger.close();
    }

    // 2. transmit via NRF24L01+
    radio.write(&data, sizeof(TelemetryPacket));

    delay(100); // 10Hz transmission rate
}