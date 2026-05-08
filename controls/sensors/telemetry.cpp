#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <SD.h>
#include <SdFatConfig.h>

// pins for Teensy & NRF24
RF24 radio(9, 10);
const byte address[6] = "00001";

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
    while (!Serial) {}

#ifdef BUILTIN_SDCARD
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD card failed!");
    }
#else
    if (!SD.begin()) {
        Serial.println("SD card failed or not present!");
    }
#endif

    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setRetries(5, 15);
    radio.setAutoAck(true);
    radio.openWritingPipe(address);
    radio.stopListening();
}

void loop() {
    packet.timestamp = millis();
    packet.altitude = 1250.5;
    packet.temperature = 22.4;
    packet.pitch = 5.2;
    packet.roll = 1.1;
    packet.latitude = 36.9741;
    packet.longitude = -122.0308;

    File dataFile = SD.open("flight.csv", FILE_WRITE);
    if (dataFile) {
        dataFile.print(packet.timestamp);
        dataFile.print(",");
        dataFile.print(packet.altitude);
        dataFile.print(",");
        dataFile.print(packet.temperature);
        dataFile.print(",");
        dataFile.print(packet.pitch);
        dataFile.print(",");
        dataFile.print(packet.roll);
        dataFile.print(",");
        dataFile.print(packet.latitude, 6);
        dataFile.print(",");
        dataFile.println(packet.longitude, 6);
        dataFile.close();
    }

    bool success = radio.write(&packet, sizeof(packet));
    if (success) {
        Serial.println("Packet sent successfully");
    } else {
        Serial.println("Packet send failed");
    }

    delay(100); // 10Hz transmission rate
}
