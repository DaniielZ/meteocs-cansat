#include "core/log.h"
void Log::init(Cansat &cansat)
{
    // initilise pc serial
    Serial.begin(cansat.config.BAUDRATE);
    if (cansat.config.WAITFORPC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }
    // initilise flash

    // initilise lora
}
void Log::info()
{
}
void Log::data()
{
}