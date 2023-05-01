#pragma once
#include "core/cansat.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <SoftwareSerial.h>
#include <Adafruit_LIS2MDL.h>
class Sensor_manager
{
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SoftwareSerial _gps_serial();
    // MAGNETO WIRE0s
    TwoWire _magneto_wire();
    Adafruit_LIS2MDL _magneto;
    // BARO WIRE1
    TwoWire _baro_wire();
    MS5611 _baro;
    // HUMIDITY WIRE1
    TwoWire _humidity_wire();
    Adafruit_SHTC3 _humidity;
    void read_gps();
    void read_magneto();
    void read_baro(Cansat &cansat);
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
        float acc[3];       // in m/s
        float baro_height;  // m
        float pressure;     // Pa
        float temperature;  // C
        float humidity;     // %
        float light;        // should be 0-255
        unsigned long time; // ms
    };
    String header = "gps_lng, gps_lat, gps_height, gps_count, mag_x, mag_y, mag_z, acc_x, acc_y, acc_z, baro_height, baro, temp, humid, light, time";
    Sensor_data data;
    void init(Cansat &cansat);
    void read_data(Cansat &cansat);
};