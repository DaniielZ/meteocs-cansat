#include <Arduino.h>
#include "core/cansat.h"

void setup()
{
    Cansat cansat;
    cansat.start_states();
}

void loop()
{
    // not used for this programm
}

// if you want to configure the cansat go to INCLUDE/config.h