#include <Wire.h>
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
    .TXPOWER = 14,
    .SPREADING = 9,
    .CODING_RATE = 7,
    .SIGNAL_BW = (long)1600E3,
    .SPI = &SPI1};

SX1280 lora = new Module(LORA2400.CS, LORA2400.DIO1, LORA2400.RESET, LORA2400.DIO1, *LORA2400.SPI); // busy pin doesnt coutn
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }

    // RANGING lora
    LORA2400.SPI->setRX(LORA2400.RX);
    LORA2400.SPI->setTX(LORA2400.TX);
    LORA2400.SPI->setCS(LORA2400.CS);
    LORA2400.SPI->setSCK(LORA2400.SCK);

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
    Serial.print(F("[sx1280] Waiting for incoming transmission ... "));

    // you can receive data as an Arduino String
    String str;
    int state = lora.receive(str);

    // you can also receive data as byte array
    /*
      byte byteArr[8];
      int state = radio.receive(byteArr, 8);
    */

    if (state == RADIOLIB_ERR_NONE)
    {
        // packet was successfully received
        Serial.println(F("success!"));

        // print the data of the packet
        Serial.print(F("[sx1280] Data:\t\t"));
        Serial.println(str);

        // print RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[sx1280] RSSI:\t\t"));
        Serial.print(lora.getRSSI());
        Serial.println(F(" dBm"));
    }
    else if (state == RADIOLIB_ERR_RX_TIMEOUT)
    {
        // timeout occurred while waiting for a packet
        Serial.println(F("timeout!"));
    }
    else if (state == RADIOLIB_ERR_CRC_MISMATCH)
    {
        // packet was received, but is malformed
        Serial.println(F("CRC error!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}