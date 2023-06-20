#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>

// LORA 2.4 SPI1
struct Lora_device
{
    long FREQUENCY;
    int CS;
    int RX;
    int TX;
    int SCK;
    int DIO0; // busy pin
    int DIO1;
    int RESET;
    int SYNC_WORD;

    int TXPOWER;
    int SPREADING;
    int CODING_RATE;
    long SIGNAL_BW;
    SPIClassRP2040 *SPI;
};
Lora_device LORA2400{
    .FREQUENCY = (long)2405.6E6,
    .CS = 13,
    .RX = 12,
    .TX = 11,
    .SCK = 10,
    .DIO0 = 16, // busy pin not programmable dont use
    .DIO1 = 15, // only use thsi
    .RESET = 14,
    .SYNC_WORD = 0xF4,
    .TXPOWER = 10,
    .SPREADING = 9,
    .CODING_RATE = 7,
    .SIGNAL_BW = (long)1600E3,
    .SPI = &SPI1};

SX1280 lora = new Module(LORA2400.CS, LORA2400.DIO0, LORA2400.RESET, -1, SPI1); // busy pin doesnt coutn

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }

    // RANGING lora
    SPI1.setRX(LORA2400.RX);
    SPI1.setTX(LORA2400.TX);
    SPI1.setCS(LORA2400.CS);
    SPI1.setSCK(LORA2400.SCK);

    int state = lora.begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("SX1280 lora failed state: ");
        Serial.println(state);
    }
    else
    {
        // setting paramaters
        lora.setOutputPower(LORA2400.TXPOWER);
        lora.setSpreadingFactor(LORA2400.SPREADING);
        lora.setCodingRate(LORA2400.CODING_RATE);
        lora.setBandwidth(LORA2400.SIGNAL_BW);
        lora.setSyncWord(LORA2400.SYNC_WORD);
        Serial.println("SX1280 LoRa! Running");
    }
}

void loop()
{
    Serial.print(F("[sx1280] Transmitting packet ... "));

    // you can transmit C-string or Arduino string up to 64 characters long
    int state = lora.transmit("Hello World!");

    // you can also transmit byte array up to 64 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      int state = radio.transmit(byteArr, 8);
    */

    if (state == RADIOLIB_ERR_NONE)
    {
        // the packet was successfully transmitted
        Serial.println(F("success!"));
    }
    else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
    {
        // the supplied packet was longer than 64 bytes
        Serial.println(F("too long!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    // wait for a second before transmitting again
    delay(2000);
}