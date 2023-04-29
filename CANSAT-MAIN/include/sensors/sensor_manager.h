#pragma once
#include "core/cansat.h"
class Sensor_manager
{
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SoftwareSerial _gps_serial;
    // MAGNETO WIRE0
    TwoWire _magneto_wire;
    Adafruit_LIS2MDL _magneto;
    // BARO WIRE1
    TwoWire _baro_wire;
    MS5611 _baro;
    // HUMIDITY WIRE1
    TwoWire _humidity_wire;
    Adafruit_SHTC3 _humidity;
    void read_gps();
    void read_magneto();
    void read_baro();
    void read_humidity();

public:
    struct Sensor_data
    {
        // array data is ordered: x y z
        double gps_lng;
        double gps_lat;
        double gps_height;
        int gps_sattelites;
        // magnetic vector values are in micro-Tesla (uT)) */
        float mag[3];
        float pressure; //
        float temperature;
        float humidity; // %
    };
    Sensor_data data;
    void init(Cansat &cansat);
    void read_data();
};