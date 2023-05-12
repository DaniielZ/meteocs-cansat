#include "states/descent_state.h"
#include <core/gnc_math.h>
#include <Arduino.h>
// void toggle_parachute(Cansat &cansat, PinStatus mosfet_state)
// {
//     digitalWrite(cansat.config.MOSFET, mosfet_state);
// }
void eject_nanosat(Cansat &cansat)
{
    // open servo
    cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_END);
}

void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    eject_nanosat(cansat);
    delay(1000);

    data_point ELEMENT_COUNT_MAX[2000]; // change this if changing config
    Vector<data_point> gps_height_values(ELEMENT_COUNT_MAX);
    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);

        // check if should move to next state
        data_point current_gps_height_data_point = {cansat.sensors.data.gps_height, cansat.sensors.data.time};
        float gps_height_average = average_value(
            current_gps_height_data_point,
            cansat.config.LANDED_HEIGHT.TIMESPAN,
            gps_height_values);

        cansat.sensors.data.average_value = gps_height_average;
        if (gps_height_average <= cansat.config.LANDED_HEIGHT.THRESHOLD && gps_height_average != -1)
        {
            return;
        }

        delay(cansat.config.SLEEP);
    }
}
