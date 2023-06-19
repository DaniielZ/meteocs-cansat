#include "states/ascent_state.h"
#include "core/gnc_math.h"
#include <Arduino.h>

void ascent_state(Cansat &cansat)
{
    cansat.log.info("ascent_state");
    unsigned long ejection_time = millis() + cansat.config.TIME_FROM_LAUNCH_TO_DETECT_EJECTION;

    while (true)
    {
        unsigned long loop_start = millis();

        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);
        if (millis() > ejection_time)
        {
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
