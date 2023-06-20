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
    .TXPOWER = 10,
    .SPREADING = 6,
    .CODING_RATE = 7,
    .SIGNAL_BW = (long)62.5E3,
    .SPI = &SPI1};

SX1280 lora = new Module(LORA2400.CS, LORA2400.DIO1, LORA2400.RESET, LORA2400.DIO0, *LORA2400.SPI); // busy pin doesnt coutn

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

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
    LORA2400.SPI->setSCK(LORA2400.SCK);
    LORA2400.SPI->begin();

    int state = lora.begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("SX1280 lora failed state: ");
        Serial.println(state);
        while (true)
        {
        }
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
    // set the function that will be called
    // when new packet is received
    lora.setDio1Action(setFlag);

    // start listening for LoRa packets
    Serial.print(F("[SX1280] Starting to listen ... "));
    state = lora.startReceive();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.readData();
    // radio.scanChannel();
}

void loop()
{
    // check if the flag is set
    if (receivedFlag)
    {
        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = lora.readData(str);

        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int state = radio.readData(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            // packet was successfully received
            Serial.println(F("[SX1280] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1280] Data:\t\t"));
            Serial.println(str);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("[SX1280] RSSI:\t\t"));
            Serial.print(lora.getRSSI());
            Serial.println(F(" dBm"));

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("[SX1280] SNR:\t\t"));
            Serial.print(lora.getSNR());
            Serial.println(F(" dB"));

            // print the Frequency Error
            // of the last received packet
            Serial.print(F("[SX1280] Frequency Error:\t"));
            Serial.print(lora.getFrequencyError());
            Serial.println(F(" Hz"));
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

        // put module back to listen mode
        lora.startReceive();
    }
}
