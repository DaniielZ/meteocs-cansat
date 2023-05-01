#include "sensors/sensor_manager.h"
void Sensor_manager::init(Cansat &cansat)
{
    // GPS UART0
    _gps_serial = SoftwareSerial(cansat.config.GPS_RX, cansat.config.GPS_TX);
    _gps_serial.begin(cansat.config.GPS_BAUDRATE);
    // MAGNETO WIRE0
    _magneto_wire = TwoWire(i2c0, cansat.config.LIS2MDL_SDA, cansat.config.LIS2MDL_SCL);
    _magneto = Adafruit_LIS2MDL(12345);
    _magneto.enableAutoRange(true);
    if (!_magneto.begin(30U, &_magneto_wire))
    {
        cansat.log.error(cansat, "Magneto error");
    }
    // BARO WIRE1
    _baro_wire = TwoWire(i2c1, cansat.config.MS5611_SDA, cansat.config.MS5611_SCL);
    _baro = MS5611(cansat.config.MS5611_ADDRESS);
    if (!_baro.begin(&_baro_wire))
    {
        cansat.log.error(cansat, "Baro error");
    }
    // HUMIDITY WIRE1
    _humidity_wire = TwoWire(i2c1, cansat.config.SHTC3_SDA, cansat.config.SHTC3_SCL);
    if (!_humidity.begin(&_humidity_wire))
    {
        cansat.log.error(cansat, "Humidity error");
    }

    // Log innit complete
    cansat.log.info("Sensor innit complete");
}

void Sensor_manager::read_gps()
{
    while (_gps_serial.available() > 0)
    {
        _gps.encode(_gps_serial.read());
        if (_gps.location.isUpdated())
        {
            data.gps_lat = _gps.location.lat();
            data.gps_lng = _gps.location.lng();
            data.gps_height = _gps.altitude.meters();
            data.gps_sattelites = _gps.satellites.value();
        }
    }
}
void Sensor_manager::read_magneto()
{
    /* Get a new sensor event */
    sensors_event_t event;
    _magneto.getEvent(&event);

    /* magnetic vector values are in micro-Tesla (uT)) */
    data.mag[0] = event.magnetic.x;
    data.mag[1] = event.magnetic.y;
    data.mag[2] = event.magnetic.z;
}
void Sensor_manager::read_baro(Cansat &cansat)
{
    _baro.read(); // note no error checking => "optimistic".
    data.pressure = _baro.getPressure();
    data.baro_height = get_altitude(data.pressure, cansat.config.SEA_LEVEL_HPA);
    data.temperature = _baro.getTemperature();
}
void Sensor_manager::read_humidity()
{
    sensors_event_t humidity, temp;
    _humidity.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    data.humidity = humidity.relative_humidity;
}

void Sensor_manager::read_data(Cansat &cansat)
{
    read_gps();
    read_magneto();
    read_baro(cansat);
    read_humidity();
}

float get_altitude(float pressure_Pa, float sea_level_hPa)
{
    float altitude;
    pressure_Pa /= 100;
    altitude = 44330 * (1.0 - pow(pressure_Pa / sea_level_hPa, 0.1903));
    return altitude;
}