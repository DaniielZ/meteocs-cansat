#include <core/cansat.h>

float Cansat::average_value(float current_value, unsigned long current_time, unsigned long time_span, Vector<data_point> &data_points)
{
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
void Cansat::start_states(Cansat &cansat)
{
    current_state = State::PREP;
    prepare_state(cansat);
    current_state = State::ARMED;
    armed_state(cansat);
    current_state = State::ASCENT;
    ascent_state(cansat);
    current_state = State::DESCENT;
    descent_state(cansat);
    current_state = State::LANDED;
    landed_state(cansat);
}
