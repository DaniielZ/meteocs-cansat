#include "core/gnc_math.h"

// float average_value(data_point current_data_point, unsigned long time_span, Array<data_point> &data_points)
// {
//     data_points.push_back(current_data_point);
//     // Serial.println("Data_points size: " + String(data_points.size()));
//     // return if not enough samples gathered
//     if (data_points.size() < 2)
//     {
//         return -1;
//     }
//     // return if not enough samples gathered
//     if (data_points.back().time - data_points.front().time <= time_span)
//     {
//         return -1;
//     }
//     // remove old values
//     int i = data_points.size() - 1;
//     while (i >= 0)
//     {
//         if (data_points[i].time + time_span < current_data_point.time)
//         {
//             data_points.remove(i);
//         }
//         i--;
//     }
//     // calculate average from list
//     float value_sum = 0;
//     int size = 0;
//     for (data_point i : data_points)
//     {
//         value_sum += i.value;
//         size++;
//     }
//     float average_value = value_sum / size;
//     return average_value;
// }