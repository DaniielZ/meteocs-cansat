#include "core/log.h"

void Log::init_lora(Cansat &cansat)
{
    _spi_lora = SPIClassRP2040(spi0, cansat.config.LORA_RX, cansat.config.LORA_CS, cansat.config.LORA_SCK, cansat.config.LORA_TX);
    _lora.setPins(cansat.config.LORA_CS);
    _lora.setSPI(_spi_lora);
    if (!_lora.begin(cansat.config.LORA_FREQUENCY))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    _lora.setTxPower(cansat.config.LORA_TXPOWER);
    _lora.setSpreadingFactor(cansat.config.LORA_SPREADING);
    _lora.setCodingRate4(cansat.config.LORA_CODING_RATE);
    _lora.setSignalBandwidth(cansat.config.LORA_SIGNAL_BW);
    Serial.println("LoRa! Running");
}
void Log::init_flash(Cansat &cansat)
{
    // initilise flash
    _spi_flash = SPIClassRP2040(spi1, cansat.config.FLASH_RX, cansat.config.FLASH_CS, cansat.config.FLASH_SCK, cansat.config.FLASH_TX);
    _flash = SPIFlash(cansat.config.FLASH_CS, cansat.config.FLASH_JDEC, _spi_flash);

    gpio_pull_up(cansat.config.FLASH_WP);
    gpio_pull_up(cansat.config.FLASH_HOLD);

    if (_flash.initialize())
    {
        Serial.println("Init OK!");
    }
    else
    {
        Serial.print("Init FAIL, expectedDeviceID(0x");
        Serial.print(cansat.config.FLASH_JDEC, HEX);
        Serial.print(") mismatched the read value: 0x");
        Serial.println(_flash.readDeviceId(), HEX);
    }
}
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

    init_flash(cansat);
    init_lora(cansat);
}
void Log::info(char msg[])
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    _lora.beginPacket();
    _lora.println(msg);
    _lora.endPacket();
}
void Log::error(Cansat &cansat, char msg[])
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    _lora.beginPacket();
    _lora.println(msg);
    _lora.endPacket();
    // play error sound
    cansat.sound.error(cansat);
}
void Log::data(Cansat &cansat)
{
    // prints data
    // sends data over lora if can be sent
    // logs data to flash if apropriate state
}
bool Log::read(Cansat &cansat, String &msg)
{
    // if anything has been recieved add message to string and return true if msg was read
    int packetSize = _lora.parsePacket();

    if (packetSize)
    {
        while (_lora.available())
        {
            char incoming = (char)_lora.read();
            msg += incoming;
        }
        return true;
    }
    else
    {
        return false;
    }
}