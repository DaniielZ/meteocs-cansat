#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <LSM6.h>
#include <SoftwareSerial.h>
#include <NTC_Thermistor.h>
#include <RadioLib.h>
#include "temperature_manager.h"
#include <ranging_wrapper.h>
#include "core/data_filtering.h"
#include <Array.h>
#include "config.h"

/**
 * @brief A class responsible for initializing, managing and reading data from all the diferent sensors and controllers. All the data is stored in the data struct
 *
 */
class Sensor_manager
{

    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SerialUART *_gps_serial;
    bool _gps_initialized = false;
    unsigned long _last_gps_packet_time = 0;
    // BARO WIRE0
    MS5611 _outter_baro;
    bool _outter_baro_initialized = false;
    // BARO WIRE0 currently not in use
    Adafruit_BMP085 _inner_baro;
    bool _inner_baro_initialized = false;
    // IMU WIRE0
    LSM6 _imu;
    bool _imu_initialized = false;
    // TEMPERATURE WIRE0
    ClosedCube::Sensor::STS35 _inner_temp_probe;
    bool _inner_temp_probe_initialized = false;
    // TEMPREATURE NTC
    NTC_Thermistor _outer_thermistor = NTC_Thermistor(0, 0, 0, 0, 0);
    bool _outer_thermistor_initialized = false;
    // temp manager
    Temperature_Manager _temp_manager;
    bool _heater_enabled = false;
    Time_Averaging_Filter<float> *_inner_temp_averager;
    Time_Averaging_Filter<float> *_outer_temp_averager;
    // ranging lora
    Ranging_Wrapper _lora;
    unsigned long _last_ranging_pos_time = 0;
    int _last_slave_index = 0;
    int _slave_index = 0;
    // battery averager
    Time_Averaging_Filter<float> *_batt_averager;

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
    void read_batt_voltage(Config &config);

public:
    // TODO make a different struct for sendable data and raw data
    struct Sensor_data
    {
        // array data is ordered: x y z
        float gps_lat = 0;      // deg
        float gps_lng = 0;      // deg
        float gps_height = 0;   // m
        int gps_sattelites = 0; // count

        float inner_baro_pressure = 0; // Pa
        float inner_baro_temp = 0;     // C

        float inner_temp_probe = 0; // C
        float outter_temp_thermistor = 0;

        float average_inner_temp = 0;  // C
        float average_outter_temp = 0; // C
        float heater_power = 0;        // 0-255
        float p = 0;                   // propotional * coeffcient
        float i = 0;                   // integral * coeffcient
        float d = 0;                   // derivative * coeffcient
        float target_temp = 0;

        float humidity = 0; // %

        float acc[3] = {0, 0, 0};  // m/s^2
        float gyro[3] = {0, 0, 0}; // dps

        Ranging_Wrapper::Ranging_Result ranging_results[3];
        Ranging_Wrapper::Position ranging_position = Ranging_Wrapper::Position(0, 0, 0);

        float batt_votage = 0;          // V
        float average_batt_voltage = 0; // v

        unsigned long time = 0;                           // ms
        unsigned long time_since_last_gps = 0;            // ms
        unsigned long times_since_last_ranging_result[3]; // ms
        unsigned long time_since_last_ranging_pos = 0;    // ms
        unsigned long gps_time = 0;                       //
    };

    //[F] = not sent over lora
    void enable_heater()
    {
        _heater_enabled = true;
        _temp_manager.reset();
    };
    String header = "Data header:";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};