// #include <Arduino.h>
// #include <SPI.h>
// #include <LoRa.h>

// long MAIN_FREQUENCY = 434.1E6;
// uint8_t SPI_MAIN_CS = 5;
// uint8_t SPI_MAIN_RX = 4;
// uint8_t SPI_MAIN_TX = 3;
// uint8_t SPI_MAIN_SCK = 2;
// LoRaClass LoRaMain;

// void setup()
// {
//     Serial.begin(9600); // initialize serial
//     while (!Serial)
//     {
//         delay(100);
//     }
//     Serial.println("Starting LoRa......!");

//     // SPI LoRa pins
//     SPI.setCS(SPI_MAIN_CS);
//     SPI.setRX(SPI_MAIN_RX);
//     SPI.setTX(SPI_MAIN_TX);
//     SPI.setSCK(SPI_MAIN_SCK);

//     LoRaMain.setPins(SPI_MAIN_CS);
//     LoRaMain.setSPI(SPI);

//     if (!LoRaMain.begin(MAIN_FREQUENCY))
//     {
//         Serial.println("Starting LoRa failed!");
//         while (1)
//             ;
//     }

//     // setting paramaters
//     LoRaMain.setTxPower(10);
//     LoRaMain.setSpreadingFactor(10);
//     LoRaMain.setCodingRate4(6);
//     LoRaMain.setSignalBandwidth(62.5E3);
//     Serial.println("LoRa! Running");
// }

// void loop()
// {
//     // try to parse packet
//     int packetSize = LoRaMain.parsePacket();
//     if (packetSize)
//     {
//         // received a packet
//         Serial.print("Received packet '");

//         // read packet
//         while (LoRaMain.available())
//         {
//             Serial.print((char)LoRaMain.read());
//         }

//         // print RSSI of packet
//         Serial.print("' with RSSI ");
//         Serial.println(LoRaMain.packetRssi());
//     }
// }
#include <Arduino.h>
#include <SPI.h>
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }
}
void loop()
{
    Serial.println("hello1");
    delay(1000);
}