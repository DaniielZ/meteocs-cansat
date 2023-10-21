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
    radio.setSpreadingFactor(6);
    radio.setCodingRate(6);
    radio.setBandwidth(62.5);
}

void loop()
{
    Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

    // you can receive data as an Arduino String
    String str;
    int state = radio.receive(str);

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
        Serial.print(F("[SX1278] Data:\t\t\t"));
        Serial.println(str);

        // print the RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[SX1278] RSSI:\t\t\t"));
        Serial.print(radio.getRSSI());
        Serial.println(F(" dBm"));

        // print the SNR (Signal-to-Noise Ratio)
        // of the last received packet
        Serial.print(F("[SX1278] SNR:\t\t\t"));
        Serial.print(radio.getSNR());
        Serial.println(F(" dB"));

        // print frequency error
        // of the last received packet
        Serial.print(F("[SX1278] Frequency error:\t"));
        Serial.print(radio.getFrequencyError());
        Serial.println(F(" Hz"));
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