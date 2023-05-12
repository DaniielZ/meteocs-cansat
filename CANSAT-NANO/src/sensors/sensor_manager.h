#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <SoftwareSerial.h>
#include <Adafruit_LIS2MDL.h>
#include "sensors/H3LIS100.h"
#include "sensors/I3G4250D.h"
#include "config.h"
class Sensor_manager
{
    // SENSOR OBJECTS AND Comunication
    // GPS UART1
    TinyGPSPlus _gps;
    HardwareSerial *_gps_serial;
    bool _gps_initialized = false;
    uint8_t _serial_buffer[255];
    // MAGNETO unused
    // Adafruit_LIS2MDL _magneto;
    // bool _magneto_initialized = false;
    // Gyro WIRE0s
    // I3G4250D _gyro;
    // bool _gyro_initialized = false;
    // ACC WIRE2
    H3LIS100 *_acc;
    bool _acc_initialized = false;
    // BARO WIRE0
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
    void read_gyro();
    void read_light(Config &config);
    void read_time();

public:
    struct Sensor_data
    {
        // array data is ordered: x y z
        float gps_lng = 0;
        float gps_lat = 0;
        float gps_height = 0;
        int gps_sattelites = 0;
        float average_value = 0;
        // magnetic vector values are in micro-Tesla (uT)) */
        // float mag[3];
        float acc[3] = {0, 0, 0}; // in m/s
                                  // float gyro[3];
        float baro_height = 0;    // m
        float pressure = 0;       // Pa
        float temperature = 0;    // C
        float humidity = 0;       // %
        // float light;        // should be 0-255
        unsigned long time = 0; // ms
    };
    //[F] - not send over lora
    String header = "gps_lng, gps_lat, gps_height, gps_count, avrg_det, acc_x[F], acc_y[F], acc_z[F], baro_height, baro, temp, humid, time";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};