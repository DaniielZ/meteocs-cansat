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
void Log::info()
{
}
void Log::data()
{
}