#include "states/ascent_state.h"

unsigned long int last_data_transmit_time = 0;
void send_data(Cansat &cansat)
{

    bool data_needs_to_be_sent = false;
    if (millis() > last_data_transmit_time + cansat.config.LORA_DATAPACKET_COOLDOWN)
    {
        cansat.log.data(cansat.sensors.data, true, true);
        last_data_transmit_time = millis();
    }
    else
    {
        cansat.log.data(cansat.sensors.data, true, false);
    }
}

void ascent_state(Cansat &cansat)
{
    int launch_rail_removed_cycle_count = 0;
    while (true)
    {
        unsigned long loop_start = millis();

        send_data(cansat);
        cansat.sensors.read_data(cansat.config);

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
