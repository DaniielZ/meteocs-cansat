#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

long MAIN_FREQUENCY = 434.1E6;
uint8_t SPI_MAIN_CS = 5;
uint8_t SPI_MAIN_RX = 4;
uint8_t SPI_MAIN_TX = 3;
uint8_t SPI_MAIN_SCK = 2;
LoRaClass LoRaMain;

long NANO_FREQUENCY = 868.1E6;
uint8_t SPI_NANO_CS = 9;
uint8_t SPI_NANO_RX = 8;
uint8_t SPI_NANO_TX = 11;
uint8_t SPI_NANO_SCK = 10;
uint8_t SPI_NANO_NREST = 15;
uint8_t SPI_NANO_DIO0 = 12;
uint8_t SPI_NANO_BUSY = 13;
// for SX126x series or LLCC68
#include <SX126x.h>
SX126x LoRaNano;

void setup()
{
    Serial.begin(9600); // initialize serial
    while (!Serial)
    {
        delay(100);
    }
    Serial.println("Starting LoRa......!");

    // SPI LoRa pins
    SPI.setCS(SPI_MAIN_CS);
    SPI.setRX(SPI_MAIN_RX);
    SPI.setTX(SPI_MAIN_TX);
    SPI.setSCK(SPI_MAIN_SCK);

    LoRaMain.setPins(SPI_MAIN_CS);
    LoRaMain.setSPI(SPI);

    if (!LoRaMain.begin(MAIN_FREQUENCY))
    {
        Serial.println("Starting LoRa main failed!");
        while (1)
            ;
    }

    // setting paramaters
    LoRaMain.setTxPower(10);
    LoRaMain.setSpreadingFactor(10);
    LoRaMain.setCodingRate4(6);
    LoRaMain.setSignalBandwidth(62.5E3);
    Serial.println("LoRa main! Running");

    // SPI LoRa  nano pins
    SPI1.setCS(SPI_NANO_CS);
    SPI1.setRX(SPI_NANO_RX);
    SPI1.setTX(SPI_NANO_TX);
    SPI1.setSCK(SPI_NANO_SCK);
    LoRaNano.setSPI(SPI1);
    if (!LoRaNano.begin(SPI_NANO_CS, SPI_NANO_NREST, SPI_NANO_BUSY, -1, -1, -1))
    {
        Serial.println("Something wrong, can't begin LoRa nano radio");
        while (1)
            ;
    }
    LoRaNano.setFrequency(NANO_FREQUENCY);

    // Set RX gain to boosted gain
    Serial.println("Set RX gain to boosted gain");
    LoRaNano.setRxGain(SX126X_RX_GAIN_BOOSTED);

    // Configure modulation parameter including spreading factor (SF), bandwidth (BW), and coding rate (CR)
    Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
    uint8_t sf = 7;
    uint32_t bw = 125000;
    uint8_t cr = 5;
    LoRaNano.setLoRaModulation(sf, bw, cr);

    // Configure packet parameter including header type, preamble length, payload length, and CRC type
    Serial.println("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on");
    uint8_t headerType = SX126X_HEADER_EXPLICIT;
    uint16_t preambleLength = 12;
    uint8_t payloadLength = 15;
    bool crcType = true;
    LoRaNano.setLoRaPacket(headerType, preambleLength, payloadLength, crcType);

    // Set syncronize word for public network (0x3444)
    Serial.println("Set syncronize word to 0x3444");
    LoRaNano.setSyncWord(0x3444);

    Serial.println("\n-- LORA RECEIVER CONTINUOUS --\n");

    // Request for receiving new LoRa packet in RX continuous mode
    LoRaNano.request(SX126X_RX_CONTINUOUS);
    Serial.println("LoRa! nano Running");
}

void loop()
{
    // try to parse packet
    int packetSize = LoRaMain.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRaMain.available())
        {
            Serial.print((char)LoRaMain.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRaMain.packetRssi());
    }
    int nanoPacketSize = LoRaNano.available();
    if (nanoPacketSize)
    {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRaNano.available())
        {
            Serial.print((char)LoRaNano.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRaNano.packetRssi());
    }

    // CANSAT nano recieve

    // Check for incoming LoRa packet
    const uint8_t msgLen = LoRaNano.available();
    if (msgLen)
    {
        Serial.println("PAKETA");

        // Put received packet to message and counter variable
        char message[msgLen - 1];
        uint8_t counter;
        uint8_t i = 0;
        while (LoRaNano.available() > 1)
        {
            message[i++] = LoRaNano.read();
        }
        counter = LoRaNano.read();

        // Print received message and counter in serial
        Serial.print(message);
        Serial.print("  ");
        Serial.println(counter);

        // Print packet/signal status including package RSSI and SNR
        Serial.print("Packet status: RSSI = ");
        Serial.print(LoRaNano.packetRssi());
        Serial.print(" dBm | SNR = ");
        Serial.print(LoRaNano.snr());
        Serial.println(" dB");

        // Show received status in case CRC or header error occur
        uint8_t status = LoRaNano.status();
        if (status == SX126X_STATUS_CRC_ERR)
            Serial.println("CRC error");
        else if (status == SX126X_STATUS_HEADER_ERR)
            Serial.println("Packet header error");
        Serial.println();
    }
}
