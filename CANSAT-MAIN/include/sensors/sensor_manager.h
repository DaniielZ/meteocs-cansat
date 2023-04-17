#pragma once
#include "core/cansat.h"
class Sensor_manager
{
public:
    struct Sensor_data
    {
        // array data is ordered: x y z
        float gps_loc[3];
        float gps_speed[3];
        int gps_sattelites;
        float mag[3];
        float acc[3];
        float pressure; //
        float humidity; // %
    };
    Sensor_data data;
    void init(Cansat &cansat);
    void read_data();
};