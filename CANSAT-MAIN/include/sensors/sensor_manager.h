#pragma once
#include "core/cansat.h"
class Sensor_manager
{
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus gps;
    SoftwareSerial gps_serial;
    // MAGNETO WIRE0
    TwoWire magneto_wire;
    Adafruit_LIS2MDL magneto;
    // BARO WIRE1
    TwoWire baro_wire;
    MS5611 baro;
    // HUMIDITY WIRE1
    TwoWire humidity_wire;
    Adafruit_SHTC3 humidity;
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
        float mag[3];
        float pressure; //
        float humidity; // %
    };
    Sensor_data data;
    void init(Cansat &cansat);
    void read_data();
};