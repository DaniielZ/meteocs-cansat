#pragma once
#include <Vector.h>
struct data_point
{
    float value;
    unsigned long time;
};
float average_value(data_point current_data_point, unsigned long time_span, Vector<data_point> &data_points);