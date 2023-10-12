#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

long MAIN_FREQUENCY = 430.6E6;
uint8_t SPI_MAIN_CS = 5;  // 7
uint8_t SPI_MAIN_RX = 4;  // 6 miso
uint8_t SPI_MAIN_TX = 3;  // 5 mosi
uint8_t SPI_MAIN_SCK = 2; // 4
LoRaClass LoRaMain;

String ARM_MSG = "arm_confirm";
String DATA_MSG = "data_send";

bool transmiting_mode = false; // dont change
void init_LoRa_main()
{
    Serial.println("Starting LoRa main......!");

    // SPI LoRa pins
    SPI.setRX(SPI_MAIN_RX);
    SPI.setTX(SPI_MAIN_TX);
    SPI.setSCK(SPI_MAIN_SCK);

    LoRaMain.setPins(SPI_MAIN_CS, 0, 0);
    LoRaMain.setSPI(SPI);

    if (!LoRaMain.begin(MAIN_FREQUENCY))
    {
        Serial.println("Starting LoRa main failed!");
        return;
    }

    // setting paramaters
    LoRaMain.setTxPower(99);
    LoRaMain.setSpreadingFactor(10);
    LoRaMain.setCodingRate4(7);
    LoRaMain.setSignalBandwidth(62.5E3);
    Serial.println("LoRa main! Running");
}

void read_main_lora()
{
    // try to parse packet
    int packetSize = LoRaMain.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Main received packet '");

        // read packet
        while (LoRaMain.available())
        {
            Serial.print((char)LoRaMain.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRaMain.packetRssi());
    }
}

void send_main_lora(String msg)
{
    while (LoRaMain.beginPacket() == 0)
    {
        Serial.print("waiting for main lora ... ");
        delay(50);
    }
    // send packet
    LoRaMain.beginPacket();
    LoRaMain.print(msg);
    Serial.print("Main lora transmited: " + msg + " ....");
    LoRaMain.endPacket();
    Serial.println("done!");
}
void setup()
{
    Serial.begin(115200); // initialize serial
    while (!Serial)
    {
        delay(100);
    }
    init_LoRa_main();
}

void loop()
{
    if (transmiting_mode)
    {
        if (Serial.available() > 0)
        {
            String incoming_msg = Serial.readString();

            if (incoming_msg == "D")
            {
                send_main_lora(DATA_MSG);
                Serial.println("Switching to recieving mode");
                transmiting_mode = false;
            }
            else if (incoming_msg == "A")
            {
                send_main_lora(ARM_MSG);
                Serial.println("Switching to recieving mode");
                transmiting_mode = false;
            }
            else if (incoming_msg == "R")
            {
                Serial.println("Switching to recieving mode");
                transmiting_mode = false;
            }
            else
            {
                Serial.println("Unexpected input : " + incoming_msg);
            }
        }
    }
    else
    {
        read_main_lora();
        if (Serial.available() > 0)
        {
            String incoming_msg = Serial.readString();
            if (incoming_msg == "T")
            {
                Serial.println("Switching to transmiting mode");
                transmiting_mode = true;
            }
            else
            {
                Serial.println("Unexpected input : " + incoming_msg);
            }
        }
    }
}
