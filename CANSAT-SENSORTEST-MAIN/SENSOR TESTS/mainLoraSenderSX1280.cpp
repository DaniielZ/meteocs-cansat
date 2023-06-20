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
    .DIO0 = 18, // busy pin not programmable dont use
    .DIO1 = 15, // only use thsi
    .RESET = 14,
    .SYNC_WORD = 0xF4,
    .TXPOWER = 10,
    .SPREADING = 6,
    .CODING_RATE = 7,
    .SIGNAL_BW = (long)62.5E3,
    .SPI = &SPI1};

SX1280 lora = new Module(LORA2400.CS, LORA2400.DIO1, LORA2400.RESET, LORA2400.DIO0, *LORA2400.SPI); // busy pin doesnt coutn

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;
// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
    // we sent a packet, set the flag
    transmittedFlag = true;
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
    // when packet transmission is finished
    lora.setDio1Action(setFlag);

    // start transmitting the first packet
    Serial.print(F("[SX1280] Sending first packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = lora.startTransmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      state = radio.startTransmit(byteArr, 8);
    */
}

void loop()
{
    // check if the previous transmission finished
    if (transmittedFlag)
    {
        // reset flag
        transmittedFlag = false;

        if (transmissionState == RADIOLIB_ERR_NONE)
        {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));
        }
        else
        {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
        }

        // clean up after transmission is finished
        // this will ensure transmitter is disabled,
        // RF switch is powered down etc.
        lora.finishTransmit();

        // wait a second before transmitting again
        delay(1000);

        // send another one
        Serial.print(F("[SX1280] Sending another packet ... "));

        // you can transmit C-string or Arduino string up to
        // 256 characters long
        transmissionState = lora.startTransmit("Hello World!");

        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                            0x89, 0xAB, 0xCD, 0xEF};
          int state = radio.startTransmit(byteArr, 8);
        */
    }
}