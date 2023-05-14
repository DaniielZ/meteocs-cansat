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
    // GPS UART0
    TinyGPSPlus _gps;
    SerialUART *_gps_serial;
    bool _gps_initialized = false;
    // MAGNETO WIRE0s
    Adafruit_LIS2MDL _magneto;
    bool _magneto_initialized = false;
    // MAGNETO WIRE0s
    I3G4250D _gyro;
    bool _gyro_initialized = false;
    // ACC WIRE1
    H3LIS100 *_acc;
    bool _acc_initialized = false;
    // BARO WIRE1
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
        float gps_lng;
        float gps_lat;
        float gps_height;
        int gps_sattelites;
        float average_value; // used for detection
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
        unsigned long time_since_last_gps;
    };
    unsigned long last_gps_packet_time = 0;
    //[F] - not send over lora
    String header = "gps_lng, gps_lat, gps_height, gps_count, avrg_det, mag_x[F], mag_y[F], mag_z[F], acc_x[F], acc_y[F], acc_z[F], gyro_x[F], gyro_y[F], gyro_z[F], baro_height, baro, temp, humid, light, since_gps, time";
    Sensor_data data;
    String init(Config &config);
    void read_data(Config &config);
};