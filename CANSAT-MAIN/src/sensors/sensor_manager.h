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
    SX1280 *_lora;
    bool _lora_initialized = false;

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
        float gps_lng;
        float gps_lat;
        float gps_height; // m
        int gps_sattelites;
        float acc[3];       // m/s
        float gyro[3];      // deg or rad/s
        float total_acc;    // m/s
        float baro_height;  // m
        float pressure;     // Pa
        float temperature;  // C
        float humidity;     // %
        unsigned long time; // ms
        unsigned long time_since_last_gps;
    };

    //[F] = not sent over lora
    String header = "gps_lat, gps_lng, gps_height, gps_count, total_acc, , gyro_x, gyro_y, gyro_z, baro_height, baro, temp, humid, light, since_gps, time, acc_x[F], acc_y[F], acc_z[F]";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};