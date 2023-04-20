#include <Arduino.h>
#include <SPIFlash.h>
#include <SPI.h>
// TESTED

uint8_t SPIFLASH_CS = 13;
uint8_t SPIFLASH_RX = 12;
uint8_t SPIFLASH_TX = 11;
uint8_t SPIFLASH_SCK = 10;
uint8_t SPIFLASH_WP = 9;
uint8_t SPIFLASH_HOLD = 8;

uint16_t JDEC = 0x1F85;

SPIFlash flash(SPIFLASH_CS, JDEC);

char input = 0;
long lastPeriod = -1;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Serial.print("Init started!");

    SPI1.setTX(SPIFLASH_TX);
    SPI1.setRX(SPIFLASH_RX);
    SPI1.setCS(SPIFLASH_CS);
    SPI1.setSCK(SPIFLASH_SCK);

    gpio_pull_up(SPIFLASH_WP);
    gpio_pull_up(SPIFLASH_HOLD);

    if (flash.initialize())
    {
        Serial.println("Init OK!");
    }
    else
    {
        Serial.print("Init FAIL, expectedDeviceID(0x");
        Serial.print(JDEC, HEX);
        Serial.print(") mismatched the read value: 0x");
        Serial.println(flash.readDeviceId(), HEX);
    }

    Serial.println("\n************************");
    Serial.println("Available operations:");
    Serial.println("'c' - read flash chip's deviceID 10 times to ensure chip is present");
    Serial.println("'d' - dump first 256 bytes on the chip");
    Serial.println("'e' - erase entire flash chip");
    Serial.println("'i' - read deviceID");
    Serial.println("************************\n");
}

void loop()
{
    // Handle serial input (to allow basic DEBUGGING of FLASH chip)
    // ie: display first 256 bytes in FLASH, erase chip, write bytes at first 10 positions, etc
    if (Serial.available() > 0)
    {
        input = Serial.read();
        if (input == 'd') // d=dump flash area
        {
            Serial.println("Flash content:");
            int counter = 0;

            while (counter <= 256)
            {
                Serial.print(flash.readByte(counter++), HEX);
                Serial.print('.');
            }

            Serial.println();
        }
        else if (input == 'c')
        {
            Serial.print("Checking chip is present ... ");
            uint16_t deviceID = 0;
            for (uint8_t i = 0; i < 10; i++)
            {
                uint16_t idNow = flash.readDeviceId();
                if (idNow == 0 || idNow == 0xffff || (i > 0 && idNow != deviceID))
                {
                    deviceID = 0;
                    break;
                }
                deviceID = idNow;
            }
            if (deviceID != 0)
            {
                Serial.print("OK, deviceID=0x");
                Serial.println(deviceID, HEX);
            }
            else
                Serial.println("FAIL, deviceID is inconsistent or 0x0000/0xffff");
        }
        else if (input == 'e')
        {
            Serial.print("Erasing Flash chip ... ");
            flash.chipErase();
            while (flash.busy())
                ;
            Serial.println("DONE");
        }
        else if (input == 'i')
        {
            Serial.print("DeviceID: ");
            Serial.println(flash.readDeviceId(), HEX);
        }
        else if (input >= 48 && input <= 57) // 0-9
        {
            Serial.print("\nWriteByte(");
            Serial.print(input);
            Serial.print(")");
            flash.writeByte(input - 48, millis() % 2 ? 0xaa : 0xbb);
        }
    }

    // Periodically blink the onboard LED while listening for serial commands
    if ((int)(millis() / 500) > lastPeriod)
    {
        lastPeriod++;
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, lastPeriod % 2);
    }
}