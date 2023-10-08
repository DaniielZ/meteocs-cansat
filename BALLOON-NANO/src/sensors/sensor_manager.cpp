#include "sensors/sensor_manager.h"

float get_altitude(float pressure_hPa, float sea_level_hPa)
{
    float altitude;
    altitude = 44330 * (1.0 - pow(pressure_hPa / sea_level_hPa, 0.1903));
    return altitude;
}
String Sensor_manager::init(Config &config)
{

    String status;
    _gps_initialized = true;

    // BARO WIRE1
    _outter_baro = MS5611(config.MS5611_ADDRESS);
    if (!_outter_baro.begin(&Wire))
    {
        status += " Outter Baro error";
    }
    else
    {
        _outter_baro_initialized = true;
    }

    // Inner baro
    _inner_baro = Adafruit_BMP280(&Wire);
    if (!_inner_baro.begin(config.BMP280_ADDRESS_I2C))
    {
        status += " Inner Baro error";
    }
    else
    {
        _inner_baro_initialized = true;
        _inner_baro.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                                Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */
    }

    // HUMIDITY WIRE1
    if (!_humidity.begin(&Wire))
    {
        status += " Humidity error";
    }
    else
    {
        _humidity_initialized = true;
    }

    // IMU WIRE1
    if (!_imu.init())
    {
        status += " IMU error";
    }
    else
    {
        _imu_initialized = true;
        _imu.enableDefault();
    }
    // TEMP PROBE
    _inner_temp_probe = ClosedCube::Sensor::STS35(&Wire);
    _inner_temp_probe.address(config.STS35_ADDRESS);
    _inner_temp_probe_initialized = true;

    // TEMP CALCULATOR
    _temp_manager.init(config.HEATER_MOSFET, config.P, config.I, config.D);

    // RANGING lora
    status += _lora.init(config.LORA2400_MODE, config.LORA2400);

    return status;
}
void Sensor_manager::position_calculation(Config &config)
{
    Ranging_Wrapper::Positon result;
    if (!_lora.trilaterate_position(data.ranging_results, config.RANGING_SLAVES, result))
    {
        return;
    }
    // mybe do more processing
    data.ranging_position = result;
    _last_ranging_pos_time = millis();
}

void Sensor_manager::read_ranging(Config &config)
{
    Ranging_Wrapper::Ranging_Result result;
    if (!_lora.master_read(config.RANGING_SLAVES[_slave_index], result, config.RANGING_TIMEOUT))
    {
        return;
    }

    data.ranging_results[_slave_index] = result;

    // increment next address
    int array_length = sizeof(config.RANGING_SLAVES) / sizeof(Ranging_Wrapper::Ranging_Slave);
    if (_slave_index >= array_length - 1)
    {
        // reset index
        _slave_index = 0;
    }
    else
    {
        _slave_index++;
    }
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
            _last_gps_packet_time = millis();
            data.gps_lat = _gps.location.lat();
            data.gps_lng = _gps.location.lng();
            data.gps_height = _gps.altitude.meters();
            data.gps_sattelites = _gps.satellites.value();
        }
    }
}
void Sensor_manager::read_outter_baro(Config &config)
{
    if (_outter_baro_initialized != true)
    {
        return;
    }
    _outter_baro.read(); // note no error checking => "optimistic".
    data.outter_baro_pressure = _outter_baro.getPressure();
    data.outter_baro_height = get_altitude(data.outter_baro_pressure, config.SEA_LEVEL_HPA);
    data.outter_baro_temp = _outter_baro.getTemperature();
}
void Sensor_manager::read_inner_baro(Config &config)
{
    if (_inner_baro_initialized != true)
    {
        return;
    }
    data.inner_baro_pressure = _inner_baro.readPressure();
    data.inner_baro_temp = _inner_baro.readTemperature();
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

void Sensor_manager::read_time()
{
    data.time = millis();
    data.time_since_last_gps = data.time - _last_gps_packet_time;
    data.time_since_last_ranging_pos = data.time - _last_ranging_pos_time;
    for (int i = 0; i <= 2;)
    {
        data.times_since_last_ranging_result[i] = data.time - data.ranging_results[i].time;
        i++;
    }
}
void Sensor_manager::read_imu()
{
    if (!_imu_initialized)
    {
        return;
    }
    _imu.read();

    data.acc[0] = _imu.a.x;
    data.acc[1] = _imu.a.y;
    data.acc[2] = _imu.a.z;

    data.gyro[0] = _imu.g.x;
    data.gyro[1] = _imu.g.x;
    data.gyro[2] = _imu.g.x;
}
void Sensor_manager::read_temps(Config &config)
{
    // average values first and take into account temp limits
    data.average_inner_temp = data.inner_temp_probe;
    data.average_outter_temp = data.outter_temp_thermistor;

    _temp_manager.calculate_heater_power(data.average_inner_temp, data.average_outter_temp);
}

void Sensor_manager::read_data(Config &config)
{

    read_gps();
    read_inner_baro(config);
    read_outter_baro(config);
    read_temps(config);
    read_humidity();
    read_imu();
    read_ranging(config);
    position_calculation(config);
    read_time();
}
