#include "states/ascent_state.h"
#include "core/gnc_math.h"
#include <Arduino.h>

void ascent_state(Cansat &cansat)
{
    cansat.log.info("ascent_state");
    bool hard_locked = true;

    data_point ELEMENT_COUNT_MAX[200]; // change this if changing config
    Vector<data_point> gps_height_values(ELEMENT_COUNT_MAX);
    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);
        data_point current_gps_height_data_point = {cansat.sensors.data.gps_height, cansat.sensors.data.time};
        // check if falling
        if (hard_locked)
        {
            float gps_height_average = average_value(
                current_gps_height_data_point,
                cansat.config.HARD_LOCK_HEIGHT.TIMESPAN,
                gps_height_values);

            Serial.println("GPS HEIGHT AVERAGE VALUE:" + String(gps_height_average)); // debuging
            if (gps_height_average >= cansat.config.HARD_LOCK_HEIGHT.THRESHOLD && gps_height_average != -1)
            {
                hard_locked = false;
                cansat.log.info("hard_lock turned off");
            }
        }
        else
        {
            float gps_height_average = average_value(
                current_gps_height_data_point,
                cansat.config.EJECTION_HEIGHT.TIMESPAN,
                gps_height_values);

            Serial.println("GPS HEIGHT AVERAGE VALUE:" + String(gps_height_average)); // debuging
            if (gps_height_average <= cansat.config.EJECTION_HEIGHT.THRESHOLD && gps_height_average > -1)
            {
                return;
            }
        }
        delay(cansat.config.SLEEP);
    }
}
