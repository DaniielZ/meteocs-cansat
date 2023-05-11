#include "states/descent_state.h"
#include <Vector.h>
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
    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);

        // check if should move to next state

        Vector<Cansat::data_point> gps_height_values;
        float gps_height_average = cansat.average_value(
            cansat.sensors.data.gps_height,
            cansat.sensors.data.time,
            cansat.config.LANDED_HEIGHT.TIMESPAN,
            gps_height_values);

        Serial.println("GPS HEIGHT AVERAGE VALUE:" + String(gps_height_average));

        if (gps_height_average <= cansat.config.LANDED_HEIGHT.THRESHOLD && gps_height_average != 0)
        {

            return;
        }

        delay(cansat.config.SLEEP);
    }
}
