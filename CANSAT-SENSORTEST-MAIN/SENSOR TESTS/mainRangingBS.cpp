#include <Wire.h>
#include <RadioLib.h>

// LORA 2.4 SPI1
struct Lora_device
{
    float FREQUENCY;
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
    float SIGNAL_BW;
    SPIClassRP2040 *SPI;
};
Lora_device LORA2400{
    .FREQUENCY = (float)2405.6,
    .CS = 17,
    .RX = 16,
    .TX = 19,
    .SCK = 18,
    .DIO0 = 22, // busy pin not programmable dont use
    .DIO1 = 21, // only use thsi
    .RESET = 20,
    .SYNC_WORD = 0xF5,
    .TXPOWER = 10,
    .SPREADING = 9,
    .CODING_RATE = 7,
    .SIGNAL_BW = (float)1600,
    .SPI = &SPI};

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
    delay(5000);

    Serial.println("im alive kurva");

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
        lora.setFrequency(LORA2400.FREQUENCY);
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
    Serial.print(F("[SX1280] Ranging ... "));

    // start ranging exchange
    // range as master:             true
    // slave address:               0x12345678
    int state = lora.range(false, 0x12345671);

    // the other module must be configured as slave with the same address
    /*
      int state = radio.range(false, 0x12345678);
    */

    // if ranging calibration is known, it can be provided

    /*
      int state = radio.range(true, 0x12345678, calibration);
    */

    if (state == RADIOLIB_ERR_NONE)
    {
        // ranging finished successfully
        // Serial.println(F("success!"));
        // Serial.print(F("[SX1280] Distance:\t\t\t"));
        Serial.print(lora.getRangingResult());
        Serial.println(F(" meters (raw)"));
    }
    else if (state == RADIOLIB_ERR_RANGING_TIMEOUT)
    {
        // timed out waiting for ranging packet
        Serial.println(F("timed out!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    // wait for a second before ranging again
    delay(50);
}
