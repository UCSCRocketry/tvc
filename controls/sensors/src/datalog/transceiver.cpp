#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <SD.h>
#include "../include/datalog/transceiver.h"

// Unique pipe/address (5 byte address or 64 bits). Same on both boards.
const uint64_t RADIO_PIPE = 0xE8E8F0F0E1LL;

// Create RF24 object
RF24 radio(CE_PIN, CSN_PIN); 

// Logging to SD card
static File logFile;
static char logFilename[24];


static uint32_t txSeq = 0; // Transmission sequence number

// Call this on the sender Teensy on setup()
void txInit(unsigned long retries = 3, unsigned long delayCycles = 5) {
    radio.begin();
    radio.setRetries(retries, delayCycles); // Set retries and delay
    radio.setPALevel(RF24_PA_LOW); // Set power level
    radio.setAutoAck(true); // Enable auto acknowledgment
    radio.openWritingPipe(RADIO_PIPE); // Open writing pipe
    radio.stopListening(); // Set as transmitter (send only)


    // Initialize SD card and open log file
    //if (!SD.begin(SD_CS_PIN)) {
    //    Serial.println("SD card initialization failed!");
    //} else {
    //    snprintf(logFilename, sizeof(logFilename), "LOG%lu.CSV", millis());
    //    logFile = SD.open(logFilename, FILE_WRITE);
    //    if (!logFile) {
    //        Serial.println("Failed to open log file!");
    //    } else {
    //        // If file is new, write CSV header
    //        if (logFile.size() == 0) {
    //            logFile.println("seq,altitude,ms\n");
    //            logFile.flush();
    //        }
    //        Serial.print("Logging to: "); Serial.println(logFilename);
    //    }
    //}
}

// Call this on the ground (receiver) Teensy on setup()
void rxInit() {
    radio.begin();
    radio.setPALevel(RF24_PA_LOW); 
    radio.setAutoAck(true);
    radio.openReadingPipe(1, RADIO_PIPE); // Open reading pipe
    radio.startListening(); // Set as receiver (listen only)
}

// Send telemetry from rocket. Returns true if ACK received.
// TODO: update fields as needed
bool sendTelemetry(Telemetry& t) {
    t.seq = ++txSeq;
    t.ms  = (uint32_t)millis();

    radio.stopListening();
    bool ok = radio.write(&t, sizeof(t));
    // resume listening if needed
    // radio.startListening(); // Uncomment if switching back to RX mode


    // Append to log file if open
    if (logFile) {
        logFile.print(t.seq); logFile.print(",");
        logFile.print(t.ms); logFile.print(",");
        logFile.print(t.altitude, 3); logFile.print(",");
        logFile.print(t.temperature, 2); logFile.print(",");
        logFile.print(t.pitch, 3); logFile.print(",");
        logFile.print(t.roll, 3); logFile.print(",");
        logFile.print(t.latitude, 6); logFile.print(",");
        logFile.println(t.longitude, 6);
        logFile.flush();
    }

    return ok;
}

// Call frequently on ground (receiver) Teensy loop() to process incoming packets.
// Print to Serial if packet arrives.
// TODO: update fields as needed
void processIncomingTelemetry() {
    if (!radio.available()) return;

    Telemetry t;
    while (radio.available()) {
        radio.read(&t, sizeof(t));
        Serial.print("seq="); Serial.print(t.seq);
        Serial.print(" ms="); Serial.print(t.ms);
        Serial.print(" alt="); Serial.print(t.altitude, 3);
        Serial.print(" temp="); Serial.print(t.temperature, 2);
        Serial.print(" pitch="); Serial.print(t.pitch, 3);
        Serial.print(" roll="); Serial.print(t.roll, 3);
        Serial.print(" lat="); Serial.print(t.latitude, 6);
        Serial.print(" lon="); Serial.println(t.longitude, 6);
    }
}

// Call this on shutdown to close log file
void closeLogFile() {
    if (logFile) {
        logFile.close();
    }
}