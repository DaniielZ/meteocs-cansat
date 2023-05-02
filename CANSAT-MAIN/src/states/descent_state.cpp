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
    cansat.ejection_servo.write(cansat.config.SERVO_END_POS);
}
float landing_average_value(float current_value, unsigned long current_time, unsigned long time_span)
{
    struct data_point
    {
        float value;
        unsigned long time;
    };
    static Vector<data_point> data_points;
    data_point current_data_point{
        current_value, current_time};

    data_points.push_back(current_data_point);
    if (data_points.size() < 2)
    {
        return 0;
    }

    if (data_points.back().time - data_points.front().time <= time_span)
    {
        return 0;
    }

    float value_sum = 0;
    int size = 0;
    for (data_point i : data_points)
    {
        value_sum += i.value;
        size++;
    }
    float average_value = value_sum / size;
    data_points.remove(0);
    return average_value;
}
float landing_get_vector_length(float x, float y, float z)
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    eject_nanosat(cansat);
    toggle_parachute(cansat, HIGH);

    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, true);

        // check if should move to next state
        float total_acc = landing_get_vector_length(cansat.sensors.data.acc[0], cansat.sensors.data.acc[1], cansat.sensors.data.acc[2]);
        float average_total_acc = landing_average_value(total_acc, cansat.sensors.data.time, cansat.config.LANDING_TIME_SPAN);

        if (average_total_acc > cansat.config.LANDING_TRESHOLD_ACC_MIN && average_total_acc < cansat.config.LANDING_TRESHOLD_ACC_MAX)
        {
            toggle_parachute(cansat, LOW);
            return;
        }

        delay(cansat.config.SLEEP);
    }
}
