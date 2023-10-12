#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LSM6.h>
#include <SoftwareSerial.h>
#include <RadioLib.h>
#include "temperature_manager.h"
#include <Array.h>
#include "config.h"
class Sensor_manager
{
    unsigned long _last_gps_packet_time = 0;
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SerialUART *_gps_serial;
    bool _gps_initialized = false;
    // BARO WIRE0
    MS5611 _outter_baro;
    bool _outter_baro_initialized = false;
    // BARO WIRE0
    Adafruit_BMP280 _inner_baro;
    bool _inner_baro_initialized = false;
    // HUMIDITY WIRE0
    Adafruit_SHTC3 _humidity;
    bool _humidity_initialized = false;
    // IMU WIRE0
    LSM6 _imu;
    bool _imu_initialized = false;
    // TEMPERATURE WIRE0
    ClosedCube::Sensor::STS35 _inner_temp_probe;
    bool _inner_temp_probe_initialized = false;
    // temp manager
    Temperature_Manager _temp_manager;
    // ranging lora
    Ranging_Wrapper _lora;
    unsigned long _last_ranging_pos_time;
    int _slave_index = 0;

    void position_calculation(Config &config);
    void read_ranging(Config &config);
    void read_gps();
    void read_magneto();
    void read_outter_baro(Config &config);
    void read_inner_baro(Config &config);
    void read_humidity();
    void read_imu();
    void read_time();
    void read_temps(Config &config);

public:
    struct Sensor_data
    {
        // array data is ordered: x y z
        float gps_lat = 0;
        float gps_lng = 0;
        float gps_height = 0; // m
        int gps_sattelites = 0;

        // float outter_baro_height = 0; // m
        // float outter_baro_temp = 0;
        // float outter_baro_pressure = 0; // Pa

        float inner_baro_pressure = 0;
        float inner_baro_temp = 0;

        float inner_temp_probe = 0;
        float outter_temp_thermistor = 0;

        float average_inner_temp = 0;  // C  averaged
        float average_outter_temp = 0; // C averaged
        float heater_power = 0;        // TBD

        float humidity = 0; // %

        float acc[3];
        float gyro[3];

        Ranging_Wrapper::Ranging_Result ranging_results[3];
        Ranging_Wrapper::Position ranging_position;

        unsigned long time = 0; // ms
        unsigned long time_since_last_gps = 0;
        unsigned long times_since_last_ranging_result[3];
        unsigned long time_since_last_ranging_pos = 0;
        unsigned long gps_time = 0;
    };

    //[F] = not sent over lora
    String header = "Data header:";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};