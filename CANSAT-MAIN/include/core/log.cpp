#include "core/log.h"
void Log::init(Cansat &cansat)
{
    // initilise pc serial
    Serial.begin(cansat.config.PC_BAUDRATE);
    if (cansat.config.WAIT_PC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }

    // initilise flash
    SPI_FLASH = SPIClassRP2040(spi1, cansat.config.FLASH_RX, cansat.config.FLASH_CS, cansat.config.FLASH_SCK, cansat.config.FLASH_TX);
    flash = SPIFlash(cansat.config.FLASH_CS, cansat.config.FLASH_JDEC, SPI_FLASH);

    gpio_pull_up(cansat.config.FLASH_WP);
    gpio_pull_up(cansat.config.FLASH_HOLD);

    if (flash.initialize())
    {
        Serial.println("Init OK!");
    }
    else
    {
        Serial.print("Init FAIL, expectedDeviceID(0x");
        Serial.print(cansat.config.FLASH_JDEC, HEX);
        Serial.print(") mismatched the read value: 0x");
        Serial.println(flash.readDeviceId(), HEX);
    }

    // initilise lora
    SPI_LORA = SPIClassRP2040(spi0, cansat.config.LORA_RX, cansat.config.LORA_CS, cansat.config.LORA_SCK, cansat.config.LORA_TX);
    LoRaMain.setPins(cansat.config.LORA_CS);
    LoRaMain.setSPI(SPI_LORA);
    if (!LoRaMain.begin(cansat.config.LORA_FREQUENCY))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    LoRaMain.setTxPower(cansat.config.LORA_TXPOWER);
    LoRaMain.setSpreadingFactor(cansat.config.LORA_SPREADING);
    LoRaMain.setCodingRate4(cansat.config.LORA_CODING_RATE);
    LoRaMain.setSignalBandwidth(cansat.config.LORA_SIGNAL_BW);
    Serial.println("LoRa! Running");
}
void Log::info(char msg[])
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    LoRaMain.beginPacket();
    LoRaMain.println(msg);
    LoRaMain.endPacket();
}
void Log::error(Cansat &cansat, char msg[])
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    LoRaMain.beginPacket();
    LoRaMain.println(msg);
    LoRaMain.endPacket();
    // play error sound
    cansat.sound.error(cansat);
}
void Log::data(Cansat &cansat, bool log_to_flash = true)
{
    // prints data
    // sends data over lora if can be sent
    // logs data to flash if apropriate state
}
bool Log::read(Cansat &cansat, String &msg)
{
    // if anything has been recieved add message to string and return true
    int packetSize = LoRaMain.parsePacket();

    if (packetSize)
    {
        while (LoRaMain.available())
        {
            char incoming = (char)LoRaMain.read();
            msg += incoming;
        }
        return true;
    }
    else
    {
        return false;
    }
}