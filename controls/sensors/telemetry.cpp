#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <SD.h>
#include <SdFatConfig.h>

// pins for Teensy 4.1 & NRF24
RF24 radio(9, 10); 
const byte address[6] = "00001";

// full Telemetry Structure based on Project Tracker BOM 
struct TelemetryPacket {
    uint32_t timestamp;
    float altitude;    // BMP3XX 
    float temperature; // BMP3XX 
    float pitch;       // BNO055 IMU 
    float roll;        // BNO055 IMU 
    float latitude;    // GPS 
    float longitude;   // GPS 
};

TelemetryPacket packet;

void setup() {
    Serial.begin(115200);
    
    // Initialize SD for CSV logging
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD card failed!");
    }

    // Initialize Radio
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.stopListening();
}

void loop() {
    // hardcoded sensor data
    // will be replacing these with actual sensor.read() calls later
    packet.timestamp = millis();
    packet.altitude = 1250.5;   // Simulated altitude (m)
    packet.temperature = 22.4;  // Simulated temp (C)
    packet.pitch = 5.2;         // Simulated tilt
    packet.roll = 1.1;
    packet.latitude = 36.9741;  // UCSC Latitude
    packet.longitude = -122.0308; // UCSC Longitude

    // 1. log to CSV
    File dataFile = SD.open("flight.csv", FILE_WRITE);
    if (dataFile) {
        dataFile.print(packet.timestamp); dataFile.print(",");
        dataFile.print(packet.altitude);  dataFile.print(",");
        dataFile.print(packet.pitch);     dataFile.print(",");
        dataFile.println(packet.latitude);
        dataFile.close();
    }

    // 2. broadcast via NRF24L01+
    bool success = radio.write(&packet, sizeof(TelemetryPacket));
    
    if (success) {
        Serial.println("Packet sent successfully");
    }

    delay(100); // 10Hz transmission rate
}