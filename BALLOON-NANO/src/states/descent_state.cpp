#include "states/descent_state.h"

unsigned long int last_data_transmit_time_descent = 0;
void send_data_descent(Cansat &cansat)
{

    bool data_needs_to_be_sent = false;
    if (millis() > last_data_transmit_time_descent + cansat.config.LORA_DATAPACKET_COOLDOWN)
    {
        cansat.log.data(cansat.sensors.data, true, true);
        last_data_transmit_time_descent = millis();
    }
    else
    {
        cansat.log.data(cansat.sensors.data, true, false);
    }
}

void descent_state(Cansat &cansat)
{
    unsigned long state_start_time = millis();
    unsigned long parachute_ejection_time = state_start_time + cansat.config.TIME_FROM_LAUNCH_TO_EJECT;
    bool mosfet_on_triggered = false;
    unsigned long mosfet_turnoff_time = parachute_ejection_time + cansat.config.MOSFET_ON_TIME;
    while (true)
    {
        unsigned long loop_start = millis();

        cansat.sensors.read_data(cansat.config);
        send_data_descent(cansat);

        if (millis() >= parachute_ejection_time && !mosfet_on_triggered)
        {
            pinMode(cansat.config.PARACHUTE_MOSFET, OUTPUT_12MA);
            digitalWrite(cansat.config.PARACHUTE_MOSFET, HIGH);
            mosfet_on_triggered = true;
        }
        if (millis() >= mosfet_turnoff_time)
        {
            pinMode(cansat.config.PARACHUTE_MOSFET, OUTPUT_12MA);
            digitalWrite(cansat.config.PARACHUTE_MOSFET, LOW);
        }

        // check if should wait before next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
