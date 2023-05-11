#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <SPI.h> // include libraries

// SEMS to be sending but not verified

long NANO_FREQUENCY = 432.25E6;
uint8_t SPI_NANO_CS = 6;
uint8_t SPI_NANO_RX = 8;
uint8_t SPI_NANO_TX = 7;
uint8_t SPI_NANO_SCK = 27;

int counter = 0;
LoRaClass LoRaNano;

void setup()
{
    Serial.begin(115200); // initialize serial
    // while (!Serial)
    // {
    //     delay(100);
    // }
    Serial.println("Starting LoRa......!");

    // SPI LoRa pins
    SPI.setCS(SPI_NANO_CS);
    SPI.setMISO(SPI_NANO_RX);
    SPI.setMOSI(SPI_NANO_TX);
    SPI.setSCK(SPI_NANO_SCK);

    LoRaNano.setPins(SPI_NANO_CS);
    LoRaNano.setSPI(SPI);

    if (!LoRaNano.begin(NANO_FREQUENCY))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    LoRaNano.setTxPower(10);
    LoRaNano.setSpreadingFactor(10);
    LoRaNano.setCodingRate4(6);
    LoRaNano.setSignalBandwidth(62.5E3);
    Serial.println("LoRa! Running");
}

void loop()
{
    while (LoRaNano.beginPacket() == 0)
    {
        Serial.print("waiting for radio ... ");
        delay(100);
    }
    Serial.print("Sending packet non-blocking: ");
    Serial.println(counter);

    // send packet
    LoRaNano.beginPacket();
    LoRaNano.print("hello nano ");
    Serial.print("hello nano ");
    LoRaNano.print(counter);
    Serial.print(counter);
    LoRaNano.endPacket();
    Serial.println("ended ");
    counter++;

    delay(1000);
}
