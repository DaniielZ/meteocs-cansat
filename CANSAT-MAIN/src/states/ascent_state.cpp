#include "states/ascent_state.h"
#include "core/gnc_math.h"
#include <Arduino.h>

void ascent_state(Cansat &cansat)
{
    cansat.log.info("ascent_state");
    bool hard_locked = true;

    data_point ELEMENT_COUNT_MAX[200];                 // change this if changing config
    Vector<data_point> data_values(ELEMENT_COUNT_MAX); // will change

    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);
        data_point current_gps_height_data_point = {cansat.sensors.data.gps_height, cansat.sensors.data.time};
        data_point current_light_data_point = {cansat.sensors.data.light, cansat.sensors.data.time};
        // check if falling
        if (hard_locked)
        {
            float gps_height_average = average_value(
                current_gps_height_data_point,
                cansat.config.HARD_LOCK_HEIGHT.TIMESPAN,
                data_values);
            cansat.sensors.data.average_value = gps_height_average;
            if (gps_height_average >= cansat.config.HARD_LOCK_HEIGHT.THRESHOLD && gps_height_average != -1)
            {
                hard_locked = false;
                data_values.clear();
                cansat.log.info("hard_lock turned off");
            }
        }
        else
        {
            float light_average = average_value(
                current_light_data_point,
                cansat.config.PARACHUTE_LIGHT.TIMESPAN,
                data_values);
            cansat.sensors.data.average_value = light_average; // so it can be logged
            if (light_average >= cansat.config.PARACHUTE_LIGHT.THRESHOLD && light_average > -1)
            {
                return;
            }
        }
        delay(cansat.config.SLEEP);
    }
}
