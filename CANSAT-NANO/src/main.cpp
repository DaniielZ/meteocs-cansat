#include "core/cansat.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(500);
    }

    Cansat cansat;
    cansat.start_states(cansat);
}

void loop()
{
    // not used for this programm
}

// if you want to configure the cansat go to INCLUDE/config.h
// The program lacks a lot of error checking