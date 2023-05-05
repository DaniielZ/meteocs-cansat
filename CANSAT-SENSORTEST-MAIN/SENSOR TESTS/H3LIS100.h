// class definition of 100g acceleromter H3LIS100
// using adafruit unified sensor driver
// matt ruffner Fall 2021

#ifndef H3LIS100_H
#define H3LIS100_H

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Arduino.h>

#define H3LIS100_G_PER_LSB 0.78f

#define H3LIS100_ADDRESS 0x18

#define H3LIS100_WHOAMI 0x0F
#define H3LIS100_WHOAMI_VALUE 0x32

#define H3LIS100_CTRL_REG1 0x20
#define H3LIS100_OUT_X 0x29
#define H3LIS100_OUT_Y 0x2B
#define H3LIS100_OUT_Z 0x2D

#include <Adafruit_Sensor.h>

class H3LIS100 : public Adafruit_Sensor
{

public:
    /* Constructor */
    H3LIS100(int32_t);

    bool begin(uint8_t i2c_addr = H3LIS100_ADDRESS, TwoWire *theWire = &Wire);
    bool getEvent(sensors_event_t *);
    void getSensor(sensor_t *);

private:
    int32_t _sensorID;
    Adafruit_I2CDevice *i2c_dev;
    Adafruit_I2CRegister *_config_reg;
};

#endif