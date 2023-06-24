#include <Wire.h>
#include <RadioLib.h>

RFM96 radio = new Module(5, 7, 6, 8, SPI);
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    SPI.setRX(4);
    SPI.setTX(3);
    SPI.setSCK(2);
    SPI.begin();
    int state = radio.begin();
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
    radio.setFrequency(430.6);
    radio.setSpreadingFactor(10);
    radio.setCodingRate(7);
    radio.setBandwidth(62.5);
}

void loop()
{
    Serial.print(F("[RF69] Transmitting packet ... "));

    // you can transmit C-string or Arduino string up to 64 characters long
    int state = radio.transmit("Hello World!");

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
    delay(1000);
}