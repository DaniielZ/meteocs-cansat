#include "states/descent_state.h"
#include <core/gnc_math.h>
#include <Arduino.h>

void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    unsigned long solar_sail_open_time = millis();
    unsigned long nanosat_ejection_time = solar_sail_open_time + cansat.config.TIME_AFTER_SOLAR_SAIL_TO_DEATACH_NANOSAT;
    unsigned long landed_time = solar_sail_open_time + cansat.config.TIME_AFTER_SOLAR_SAIL_TO_LAND;
    while (true)
    {
        unsigned long loop_start = millis();

        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);

        if (millis() > solar_sail_open_time)
        {
            digitalWrite(cansat.config.MOSFET, HIGH);
        }
        if (millis() > nanosat_ejection_time)
        {
            cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_END);
        }
        if (millis() > landed_time)
        {
            // landed
            digitalWrite(cansat.config.MOSFET, LOW);
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
