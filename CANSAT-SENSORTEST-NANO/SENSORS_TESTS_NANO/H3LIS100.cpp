// class definition of 100g acceleromter H3LIS100
// using adafruit unified sensor driver
// matt ruffner Fall 2021

#include "H3LIS100.h"

// #include <avr/pgmspace.h>
// #include <limits.h>
// #include "pins_arduino.h"
// #include "wiring_private.h"

H3LIS100::H3LIS100(int32_t sensorID)
{
    _sensorID = sensorID;
}

bool H3LIS100::begin(uint8_t i2c_addr, TwoWire *theWire)
{
    i2c_dev = new Adafruit_I2CDevice(i2c_addr, theWire);

    /* Try to instantiate the I2C device. */
    if (!i2c_dev->begin(false))
    { // *dont scan!*
        return false;
    }

    // check that the WHOAMI is okay
    Adafruit_I2CRegister id_reg = Adafruit_I2CRegister(i2c_dev, H3LIS100_WHOAMI, 1);

    if (id_reg.read() != H3LIS100_WHOAMI_VALUE)
    {
        return false;
    }

    // define the config register
    _config_reg = new Adafruit_I2CRegister(i2c_dev, H3LIS100_CTRL_REG1);
    _config_reg->write(0x27); // ODR=ODR (default 50Hz), all axes enabled

    // TODO: better check if actually enabled??
    return true;
}

bool H3LIS100::getEvent(sensors_event_t *event)
{

    // get the values from the accelerometer
    float x, y, z;

    Adafruit_I2CRegister xreg = Adafruit_I2CRegister(i2c_dev, H3LIS100_OUT_X, 1);
    x = (int8_t)xreg.read() * H3LIS100_G_PER_LSB;

    Adafruit_I2CRegister yreg = Adafruit_I2CRegister(i2c_dev, H3LIS100_OUT_Y, 1);
    y = (int8_t)yreg.read() * H3LIS100_G_PER_LSB;

    Adafruit_I2CRegister zreg = Adafruit_I2CRegister(i2c_dev, H3LIS100_OUT_Z, 1);
    z = (int8_t)zreg.read() * H3LIS100_G_PER_LSB;

    /* Clear the event */
    memset(event, 0, sizeof(sensors_event_t));

    event->version = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type = SENSOR_TYPE_ACCELEROMETER;
    event->timestamp = 0;
    event->acceleration.x = x;
    event->acceleration.y = y;
    event->acceleration.z = z;

    return true;
}

void H3LIS100::getSensor(sensor_t *sensor)
{
    /* Clear the sensor_t object */
    memset(sensor, 0, sizeof(sensor_t));

    /* Insert the sensor name in the fixed length char array */
    strncpy(sensor->name, "H3LIS100", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = 0;
    sensor->version = 1;
    sensor->sensor_id = _sensorID;
    sensor->type = SENSOR_TYPE_ACCELEROMETER;
    sensor->min_delay = 0;
    sensor->max_value = 100.0F; // -100 to 100 g
    sensor->min_value = -100.0F;
    sensor->resolution = 0.780F; // 0.780 mg resolution

    /* Clear the reserved section ??? */
    // memset(sensor->reserved0, 0, sizeof(sensor->reserved0));
}
