#include <Arduino.h>
#include <SPI.h> // include libraries
#include <Wire.h>
#include <SX126x.h>
// SEMS to be sending but not verified

long NANO_FREQUENCY = 868.1E6;
uint8_t SPI_NANO_CS = 5;
uint8_t SPI_NANO_RX = 8;
uint8_t SPI_NANO_TX = 7;
uint8_t SPI_NANO_SCK = 27;
uint8_t SPI_NANO_NREST = 16;
uint8_t SPI_NANO_DIO0 = 25; // unused
uint8_t SPI_NANO_BUSY = 26;

SX126x LoRa;

// Message to transmit
char message[] = "HeLoRa World!";
uint8_t nBytes = sizeof(message);
uint8_t counter = 0;

void setup()
{

    // Begin serial communication
    Serial.begin(38400);
    while (!Serial)
    {
        delay(100);
    }

    SPI.setCS(SPI_NANO_CS);
    SPI.setMISO(SPI_NANO_RX);
    SPI.setMOSI(SPI_NANO_TX);
    SPI.setSCK(SPI_NANO_SCK);
    LoRa.setSPI(SPI);
    LoRa.setPins(SPI_NANO_CS, SPI_NANO_NREST, SPI_NANO_BUSY);
    Serial.println("Begin LoRa radio");
    if (!LoRa.begin())
    {
        Serial.println("Something wrong, can't begin LoRa radio");
        while (1)
            ;
    }
    LoRa.setFrequency(NANO_FREQUENCY);
    LoRa.setTxPower(17);

    uint8_t sf = 7;       // LoRa spreading factor: 7
    uint32_t bw = 125000; // Bandwidth: 125 kHz
    uint8_t cr = 5;       // Coding rate: 4/5
    LoRa.setLoRaModulation(sf, bw, cr);
    uint8_t headerType = SX126X_HEADER_EXPLICIT; // Explicit header mode
    uint16_t preambleLength = 12;                // Set preamble length to 12
    uint8_t payloadLength = 15;                  // Initialize payloadLength to 15
    bool crcType = true;                         // Set CRC enable
    LoRa.setLoRaPacket(headerType, preambleLength, payloadLength, crcType);

    LoRa.setSyncWord(0x3444);

    Serial.println("\n-- LORA TRANSMITTER --\n");
}

void loop()
{

    // Transmit message and counter
    // write() method must be placed between beginPacket() and endPacket()
    LoRa.beginPacket();
    LoRa.write(message, nBytes);
    LoRa.write(counter);
    LoRa.endPacket();

    // Print message and counter in serial
    Serial.print(message);
    Serial.print("  ");
    Serial.println(counter++);

    // Wait until modulation process for transmitting packet finish
    LoRa.wait();

    // Print transmit time
    Serial.print("Transmit time: ");
    Serial.print(LoRa.transmitTime());
    Serial.println(" ms");
    Serial.println();

    // Don't load RF module with continous transmit
    delay(5000);
}