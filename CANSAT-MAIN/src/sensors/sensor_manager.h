#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <SoftwareSerial.h>
#include <Adafruit_LIS2MDL.h>
#include "config.h"
class Sensor_manager
{
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SoftwareSerial *_gps_serial;
    bool _gps_initialized = false;
    // MAGNETO WIRE0s
    Adafruit_LIS2MDL _magneto;
    bool _magneto_initialized = false;
    // BARO WIRE1
    TwoWire *_baro_wire;
    MS5611 _baro;
    bool _baro_initialized = false;
    // HUMIDITY WIRE1
    Adafruit_SHTC3 _humidity;
    bool _humidity_initialized = false;

    void read_gps();
    void read_magneto();
    void read_baro(Config &config);
    void read_humidity();
    void read_acc();
    void read_time();

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
        float acc[3]; // in m/s
        float gyro[3];
        float baro_height;  // m
        float pressure;     // Pa
        float temperature;  // C
        float humidity;     // %
        float light;        // should be 0-255
        unsigned long time; // ms
    };
    String header = "gps_lng, gps_lat, gps_height, gps_count, mag_x, mag_y, mag_z, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, baro_height, baro, temp, humid, light, time";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};