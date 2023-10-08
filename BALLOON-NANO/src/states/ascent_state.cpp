#include "states/ascent_state.h"

void ascent_state(Cansat &cansat)
{
    int launch_rail_removed_cycle_count = 0;
    while (true)
    {
        unsigned long loop_start = millis();

        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true, true);

        if (digitalRead(cansat.config.LAUNCH_RAIL_SWITCH))
        {
            launch_rail_removed_cycle_count++;
            if (launch_rail_removed_cycle_count >= 10)
            {
                cansat.log.info("Launch rail removed");
                return;
            }
        }
        else
        {
            launch_rail_removed_cycle_count = 0;
        }

        // check if should wait before next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
