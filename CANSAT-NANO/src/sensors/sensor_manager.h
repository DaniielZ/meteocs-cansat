#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MS5611.h>
#include <Adafruit_SHTC3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SoftwareSerial.h>
#include <RadioLib.h>
#include "config.h"
class Sensor_manager
{
    unsigned long _last_gps_packet_time = 0;
    // SENSOR OBJECTS AND Comunication
    // GPS UART0
    TinyGPSPlus _gps;
    SerialUART *_gps_serial;
    bool _gps_initialized = false;
    // BARO WIRE1
    MS5611 _baro;
    bool _baro_initialized = false;
    // HUMIDITY WIRE1
    Adafruit_SHTC3 _humidity;
    bool _humidity_initialized = false;
    // IMU WIRE1
    Adafruit_BNO055 _imu;
    bool _imu_initialized = false;
    // RANGING LORA SPI1
    SX1280 _lora = new Module(13, 15, 14, 18, SPI1);
    bool _lora_initialized = false;
    bool _lora_wait_for_othersat = false;
    unsigned long _ranging_start_time = 0;
    unsigned long _wait_for_othersat_start_time = 0;
    int _lora_slave_address_index = 0;
    int _lora_range_state;

    void enable_ranging(Config &config);
    void read_gps();
    void read_magneto();
    void read_baro(Config &config);
    void read_humidity();
    void read_imu();
    void read_time();

public:
    struct Sensor_data
    {
        // array data is ordered: x y z
        float gps_lng = 0;
        float gps_lat = 0;
        float gps_height = 0; // m
        int gps_sattelites = 0;
        float acc[3];          // m/s
        float gyro[3];         // deg or rad/s
        float total_acc = 0;   // m/s
        float baro_height = 0; // m
        float pressure = 0;    // Pa
        float temperature = 0; // C
        float humidity = 0;    // %
        long ranging_address = 0;
        float ranging_result = 0;
        unsigned long time = 0; // ms
        unsigned long time_since_last_gps = 0;
    };

    //[F] = not sent over lora
    String header = "Data header:";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};