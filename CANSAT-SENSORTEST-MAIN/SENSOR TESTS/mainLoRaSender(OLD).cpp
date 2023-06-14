#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h> // include libraries
#include <Wire.h>
// SEMS to be sending but not verified
// LORA 433.1 working
long MAIN_FREQUENCY = 434.25E6;
uint8_t SPI_MAIN_CS = 5;
uint8_t SPI_MAIN_RX = 4;
uint8_t SPI_MAIN_TX = 3;
uint8_t SPI_MAIN_SCK = 2;

int counter = 0;
LoRaClass LoRaMain;

void setup()
{
    Serial.begin(115200); // initialize serial
    delay(2000);
    // while (!Serial)
    // {
    //     delay(100);
    // }
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
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    LoRaMain.setTxPower(10);
    LoRaMain.setSpreadingFactor(10);
    LoRaMain.setCodingRate4(6);
    LoRaMain.setSignalBandwidth(62.5E3);
    Serial.println("LoRa! Running");
}

void loop()
{
    while (LoRaMain.beginPacket() == 0)
    {
        Serial.print("waiting for radio ... ");
        delay(100);
    }

    Serial.print("Sending packet non-blocking: ");
    Serial.println(counter);

    // send packet
    LoRaMain.beginPacket();
    LoRaMain.print("hello main ");
    Serial.print("hello main ");
    LoRaMain.print(counter);
    Serial.print(counter);
    LoRaMain.endPacket();
    Serial.println("ended ");
    counter++;

    delay(2500);
}
