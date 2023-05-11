#include "sensors/sensor_manager.h"

float get_altitude(float pressure_Pa, float sea_level_hPa)
{
    float altitude;
    pressure_Pa /= 100;
    altitude = 44330 * (1.0 - pow(pressure_Pa / sea_level_hPa, 0.1903));
    return altitude;
}

String Sensor_manager::init(Config &config)
{

    String status;

    // WIRE0
    Wire.setSCL(config.LIS2MDL_SCL);
    Wire.setSDA(config.LIS2MDL_SDA);
    // WIRE1
    Wire1.setSCL(config.SHTC3_SCL);
    Wire1.setSDA(config.SHTC3_SDA);
    // GPS UART0
    _gps_serial = new SoftwareSerial(config.GPS_RX, config.GPS_TX);
    _gps_serial->begin(config.GPS_BAUDRATE);
    _gps_initialized = true;
    // MAGNETO WIRE0
    _magneto = Adafruit_LIS2MDL(12345);
    _magneto.enableAutoRange(true);
    if (!_magneto.begin(30U, &Wire))
    {
        status += "Magneto error";
        Serial.println("Magento turned off");
    }
    else
    {
        _magneto_initialized = true;
    }

    // BARO WIRE1
    _baro = MS5611(config.MS5611_ADDRESS);
    if (!_baro.begin(&Wire1))
    {
        status += " Baro error";
        Serial.println("Baro turned off");
    }
    else
    {
        _baro_initialized = true;
    }
    // HUMIDITY WIRE1
    if (!_humidity.begin(&Wire1))
    {
        status += " Humidity error";
        Serial.println("Humidity turned off");
    }
    else
    {
        _humidity_initialized = true;
    }
    Serial.println(_baro_initialized);
    return status;
}

void Sensor_manager::read_gps()
{
    if (!_gps_initialized)
    {
        return;
    }
    while (_gps_serial->available() > 0)
    {
        _gps.encode(_gps_serial->read());
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
    if (!_magneto_initialized)
    {
        return;
    }
    sensors_event_t event;
    _magneto.getEvent(&event);

    /* magnetic vector values are in micro-Tesla (uT)) */
    data.mag[0] = event.magnetic.x;
    data.mag[1] = event.magnetic.y;
    data.mag[2] = event.magnetic.z;
}
void Sensor_manager::read_baro(Config &config)
{
    if (_baro_initialized != true)
    {
        Serial.println("Baro not init.. returning");
        return;
    }
    _baro.read(); // note no error checking => "optimistic".
    data.pressure = _baro.getPressure();
    data.baro_height = get_altitude(data.pressure, config.SEA_LEVEL_HPA);
    data.temperature = _baro.getTemperature();
}
void Sensor_manager::read_humidity()
{
    if (!_humidity_initialized)
    {
        return;
    }
    sensors_event_t humidity, temp;
    _humidity.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    data.humidity = humidity.relative_humidity;
}

void Sensor_manager::read_acc()
{
}
void Sensor_manager::read_time()
{
    data.time = millis();
}
void Sensor_manager::read_data(Config &config)
{
    data = {0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, 0, 0};
    read_gps();
    read_magneto();
    read_baro(config);
    read_humidity();
    read_time();
}
