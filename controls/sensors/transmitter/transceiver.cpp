#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// Hardware pins. Change to match board.
#define CE_PIN   7
#define CSN_PIN  8

// Unique pipe/address (5 byte address or 64 bits). Same on both boards.
const uint64_t RADIO_PIPE = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN); // Create RF24 object

// Telemetry packet (packed)
// Add fields as needed
struct Telemetry {
    uint32_t seq;
    float altitude;
    uint32_t ms; // Time in milliseconds
} __attribute__((packed));

static uint32_t txSeq = 0; // Transmission sequence number

// Call this on the sender Teensy on setup()
void txInit(unsigned long retries = 3, unsigned long delayCycles = 5) {
    radio.begin();
    radio.setRetries(retries, delayCycles); // Set retries and delay
    radio.setPALevel(RF24_PA_LOW); // Set power level
    radio.setAutoAck(true); // Enable auto acknowledgment
    radio.openWritingPipe(RADIO_PIPE); // Open writing pipe
    radio.stopListening(); // Set as transmitter (send only)
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