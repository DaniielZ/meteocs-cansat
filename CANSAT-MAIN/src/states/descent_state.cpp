#include "states/descent_state.h"
#include <core/gnc_math.h>
#include <Arduino.h>
bool turned_on = false;
void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    unsigned long mosfet_open_time = millis();
    unsigned long mosfet_close_time = mosfet_open_time + cansat.config.TIME_TO_KEEP_MOSFET_ON;
    unsigned long landed_time = mosfet_open_time + cansat.config.TIME_AFTER_SOLAR_SAIL_TO_LAND;
    while (true)
    {
        unsigned long loop_start = millis();

        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);

        // if (millis() > mosfet_open_time && !turned_on)
        // {
        //     Serial.println("ONNNNN");
        //     pinMode(cansat.config.MOSFET, OUTPUT_12MA);
        //     digitalWrite(cansat.config.MOSFET, HIGH);
        //     turned_on = true;
        // }
        if (millis() > mosfet_open_time)
        {
            Serial.println("ONNNNN");
            digitalWrite(cansat.config.MOSFET, HIGH);
        }
        if (millis() > mosfet_close_time)
        {
            Serial.println("OFFFFF");
            digitalWrite(cansat.config.MOSFET, LOW);
        }
        if (millis() > landed_time)
        {
            // landed
            return;
        }

        // check if should wait before next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
