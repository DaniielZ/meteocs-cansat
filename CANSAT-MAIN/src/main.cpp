#include "core/cansat.h"

void setup()
{
    Serial.begin(115200);
    if (!Serial)
    {
        delay(500);
    }
    delay(2000);
    Serial.println("Init started");

    // Cansat cansat;
    // cansat.start_states(cansat);
}

void loop()
{
    // not used for this programm
}

// if you want to configure the cansat go to INCLUDE/config.h
// The program lacks a lot of error checking