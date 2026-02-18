// Read https://nrf24.github.io/RF24/examples_2GettingStarted_2GettingStarted_8ino-example.html
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <SD.h>

// Hardware pins. Change to match board.
#define CE_PIN   7
#define CSN_PIN  8
#define SD_CS_PIN 10

// Create RF24 object
RF24 radio(CE_PIN, CSN_PIN); 

// Node addresses for RF24 (5 bytes)
uint8_t address[][6] = {"1Node", "2Node"};

// to use different address, we need a variable to select which one to use
bool radioNumber = 1; // 0 uses address[0], 1 uses address[1]

// role selection: true for TX (transmitter), false for RX (receiver)
bool role = 1;

// Logging to SD card
static File logFile;
static char logFilename[24];

// Telemetry packet (packed)
// Add fields as needed
struct Telemetry {
    uint32_t seq;
    float altitude;
    uint32_t ms; // Time in milliseconds
} __attribute__((packed));

static uint32_t txSeq = 0; // Transmission sequence number

// Call this in setup() to initialize radio
void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    if (!radio.begin()) {
        Serial.println("Radio hardware not responding!");
        while (1) {}
    }

    Serial.println("RF24 radio initialized");
    Serial.print(F("Radio: "));
    while (!Serial.available()) {}
    char input = Serial.parseInt();
    radioNumber = input == 1;
    Serial.print(F("radioNumber: "));
    Serial.println(radioNumber);

    Serial.println("Press 'T' for transmitter");
    // Set the TX address of the RX node into the TX pipe
    radio.stopListening(address[radioNumber]);
    // Set the RX address of the TX node into a RX pipe
    radio.openReadingPipe(1, address[!radioNumber]);

    if (!role) {
        radio.startListening(); // Set as receiver (listen only)
    }
}

// Send telemetry from rocket. Returns true if ACK received.
// TODO: update fields as needed
bool sendTelemetry(float altitude) {
    Telemetry t;
    t.seq = ++txSeq;
    t.altitude = altitude;
    t.ms = (uint32_t)millis();

    // stopListening() ensures writing
    radio.stopListening();
    bool ok = radio.write(&t, sizeof(t));
    // resume listening if needed
    // radio.startListening(); // Uncomment if switching back to RX mode

    // Append to log file if open
    if (logFile) {
        logFile.print(t.seq); logFile.print(",");
        logFile.print(t.altitude, 3); logFile.print(",");
        logFile.println(t.ms);
        logFile.flush();
    }

    return ok;
}

// Call frequently on ground (receiver) Teensy loop() to process incoming packets.
// Print to Serial if packet arrives.
// TODO: update fields as needed
void processIncoming() {
    if (!radio.available()) return;

    Telemetry t;
    while (radio.available()) {
        radio.read(&t, sizeof(t));
        Serial.print(t.seq); Serial.print(", ");
        Serial.print(t.altitude, 3); Serial.print(", ");
        Serial.println(t.ms);
    }
}

// Call this on shutdown to close log file
void closeLogFile() {
    if (logFile) {
        logFile.close();
    }
}