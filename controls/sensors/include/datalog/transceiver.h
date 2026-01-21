#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <Arduino.h>

// Hardware pins. Change to match board.
#define CE_PIN   7
#define CSN_PIN  8
#define SD_CS_PIN 10

// Unique pipe/address (5 byte address or 64 bits). Same on both boards.
extern const uint64_t RADIO_PIPE;

// SD card variables
extern char logFilename[24];

// Telemetry packet (packed)
// Add fields as needed
struct Telemetry {
    uint32_t seq;
    float altitude;
    uint32_t ms; // Time in milliseconds
} __attribute__((packed));

void txInit(unsigned long retries = 3, unsigned long delayCycles = 5);
void rxInit();
bool sendTelemetry(float altitude);
void processIncomingTelemetry();
void closeLogFile();

#endif 