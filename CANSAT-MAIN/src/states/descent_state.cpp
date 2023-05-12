#include "states/descent_state.h"
#include <core/gnc_math.h>
#include <Arduino.h>
void toggle_parachute(Cansat &cansat, PinStatus mosfet_state)
{
    digitalWrite(cansat.config.MOSFET, mosfet_state);
}
void eject_nanosat(Cansat &cansat)
{
    // open servo
    // cansat.ejection_servo.write(cansat.config.SERVO_END_POS);
}

void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    toggle_parachute(cansat, HIGH);
    delay(1000);
    toggle_parachute(cansat, LOW);

    data_point ELEMENT_COUNT_MAX[2000]; // change this if changing config
    Vector<data_point> gps_height_values(ELEMENT_COUNT_MAX);
    while (true)
    {
        cansat.sensors.read_data(cansat.config);

        // check if should move to next state
        data_point current_gps_height_data_point = {cansat.sensors.data.gps_height, cansat.sensors.data.time};
        float gps_height_average = average_value(
            current_gps_height_data_point,
            cansat.config.LANDED_HEIGHT.TIMESPAN,
            gps_height_values);

        cansat.sensors.data.average_value = gps_height_average; // so it can be logged
        cansat.log.data(cansat.sensors.data, true);

        if (gps_height_average <= cansat.config.LANDED_HEIGHT.THRESHOLD && gps_height_average != -1)
        {
            cansat.log.info("Landing detected");
            return;
        }

        delay(cansat.config.SLEEP);
    }
}
