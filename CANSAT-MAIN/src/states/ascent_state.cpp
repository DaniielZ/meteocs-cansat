#include "states/ascent_state.h"
#include <Vector.h>
#include <Arduino.h>

void ascent_state(Cansat &cansat)
{
    cansat.log.info("ascent_state");
    bool hard_locked = true;
    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);
        // check if falling
        if (hard_locked)
        {
            Vector<Cansat::data_point> gps_height_values;
            float gps_height_average = cansat.average_value(
                cansat.sensors.data.gps_height,
                cansat.sensors.data.time,
                cansat.config.HARD_LOCK_HEIGHT.TIMESPAN,
                gps_height_values);

            Serial.println("GPS HEIGHT AVERAGE VALUE:" + String(gps_height_average));
            if (gps_height_average >= cansat.config.HARD_LOCK_HEIGHT.THRESHOLD && gps_height_average != 0)
            {
                hard_locked = false;
                cansat.log.info("hard_lock turned off");
            }
        }
        else
        {
            Vector<Cansat::data_point> gps_height_values;
            float gps_height_average = cansat.average_value(
                cansat.sensors.data.gps_height,
                cansat.sensors.data.time,
                cansat.config.PARACHUTE_HEIGHT.TIMESPAN,
                gps_height_values);

            Serial.println("GPS HEIGHT AVERAGE VALUE:" + String(gps_height_average));
            if (gps_height_average <= cansat.config.PARACHUTE_HEIGHT.THRESHOLD && gps_height_average != 0)
            {
                return;
            }
        }
        delay(cansat.config.SLEEP);
    }
}
