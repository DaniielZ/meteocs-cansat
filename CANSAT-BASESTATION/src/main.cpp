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
void init_LoRa_main()
{
    Serial.println("Starting LoRa main......!");

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
}

void init_LoRa_nano()
{
    // SPI LoRa  nano pins
    SPI1.setCS(SPI_NANO_CS);
    SPI1.setRX(SPI_NANO_RX);
    SPI1.setTX(SPI_NANO_TX);
    SPI1.setSCK(SPI_NANO_SCK);
    LoRaNano.setSPI(SPI1);
    LoRaNano.setPins(SPI_NANO_CS, SPI_NANO_NREST, SPI_NANO_BUSY);
    if (!LoRaNano.begin())
    {
        Serial.println("Something wrong, can't begin LoRa nano radio");
        while (1)
            ;
    }

    LoRaNano.setFrequency(NANO_FREQUENCY);
    LoRaNano.setRxGain(SX126X_RX_GAIN_BOOSTED);
    uint8_t sf = 7;
    uint32_t bw = 125000;
    uint8_t cr = 5;
    LoRaNano.setLoRaModulation(sf, bw, cr);
    LoRaNano.setLoRaPacket(LORA_HEADER_EXPLICIT, 12, 15, true, false);
    LoRaNano.setSyncWord(0x3444);
    Serial.println("LoRa nano! Running");
}
void read_main_lora()
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
}
void read_nano_lora()
{
    LoRaNano.request();
    LoRaNano.wait();
    const uint8_t msgLen = LoRaNano.available();
    if (msgLen)
    {
        while (LoRaNano.available())
        {
            Serial.print((char)LoRaNano.read());
        }
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
void setup()
{
    Serial.begin(9600); // initialize serial
    while (!Serial)
    {
        delay(100);
    }
    init_LoRa_main();
    init_LoRa_nano();
}

void loop()
{
    read_main_lora();
    read_nano_lora();
}
