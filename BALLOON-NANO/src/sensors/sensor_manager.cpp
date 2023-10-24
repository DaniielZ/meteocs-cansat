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
    _gps_serial = &Serial1;
    if (_gps_serial)
    {
        _gps_initialized = true;
    }
    else
    {
        status += " gps error";
    }

    // BARO WIRE1
    // _outter_baro = MS5611(config.MS5611_ADDRESS);
    // if (!_outter_baro.begin(&Wire))
    // {
    //     status += " Outter Baro error";
    // }
    // else
    // {
    //     _outter_baro_initialized = true;
    // }

    // Inner baro
    _inner_baro = Adafruit_BMP085();
    if (!_inner_baro.begin(config.BMP180_ADDRESS_I2C, &Wire))
    {
        status += " baro error";
    }
    else
    {
        _inner_baro_initialized = true;
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
    //_inner_temp_probe.setRepeatability(ClosedCube::Sensor::STS35::STS35_REPEATABILITY_LOW);
    _inner_temp_probe_initialized = true;
    // test
    float test = _inner_temp_probe.readTemperature();
    if (test > 100.00 || test < -100.0 || test == 0.00)
    {
        _inner_temp_probe_initialized = false;
        status += "Inner probe error";
    }

    analogReadResolution(12);
    _outer_thermistor = NTC_Thermistor(config.THERMISTOR_PIN, config.THERMISTOR_REFERENCE_R, config.THERMISTOR_NOMINAL_R, config.THERMISTOR_NOMINAL_T, config.THERMISTOR_B, 4095);
    _outer_thermistor_initialized = true;

    // TEMP CALCULATOR
    _temp_manager.init(config.HEATER_MOSFET, config.DESIRED_HEATER_TEMP);

    // RANGING lora
    status += _lora.init(config.LORA2400_MODE, config.LORA2400);

    return status;
}

void Sensor_manager::read_batt_voltage(Config &config)
{
    pinMode(config.BATT_SENS_PIN, INPUT);
    analogReadResolution(12);
    float adc_reading = analogRead(config.BATT_SENS_PIN);
    data.batt_votage = (adc_reading / 4095.0) * config.BATT_SENS_CONVERSION_FACTOR;
}
void Sensor_manager::position_calculation(Config &config)
{
    Ranging_Wrapper::Position result;
    if (_lora.trilaterate_position(data.ranging_results, config.RANGING_SLAVES, result))
    {
        data.ranging_position = result;
        _last_ranging_pos_time = millis();
    }
    // mybe do more processing
    return;
}

void Sensor_manager::read_ranging(Config &config)
{
    // increment next address
    Ranging_Wrapper::Ranging_Result result = {0, 0};
    bool move_to_next_slave = false;
    if (_lora.master_read(config.RANGING_SLAVES[_slave_index], result, config.RANGING_TIMEOUT))
    {
        move_to_next_slave = true;
    }
    // check if something was read
    if (result.distance != 0 && result.time != 0)
    {
        data.ranging_results[_slave_index] = result;
        Serial.println(_slave_index);
    }
    // check if should move to next slave
    if (move_to_next_slave)
    {
        int array_length = 3;
        _slave_index++;
        if (_slave_index > array_length - 1)
        {
            // reset index
            _slave_index = 0;
        }
        // enable ranging for next lora already
        _lora.master_read(config.RANGING_SLAVES[_slave_index], result, config.RANGING_TIMEOUT);
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
            data.gps_time = _gps.time.value();
        }
    }
}
// void Sensor_manager::read_outter_baro(Config &config)
// {
//     if (_outter_baro_initialized != true)
//     {
//         return;
//     }
//     _outter_baro.read(); // note no error checking => "optimistic".
//     data.outter_baro_pressure = _outter_baro.getPressure();
//     data.outter_baro_height = get_altitude(data.outter_baro_pressure, config.SEA_LEVEL_HPA);
//     data.outter_baro_temp = _outter_baro.getTemperature();
// }
void Sensor_manager::read_inner_baro(Config &config)
{
    if (_inner_baro_initialized != true)
    {
        return;
    }
    data.inner_baro_pressure = _inner_baro.readPressure();
    data.inner_baro_temp = _inner_baro.readTemperature();
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
    // set and devide by range
    double acc_conversion_factor = 0.061 / 1000; // mg/LSB this changes if the total range changes  currently 2g after that devide by 1000 to get m/s^2
    data.acc[0] = _imu.a.x * acc_conversion_factor;
    data.acc[1] = _imu.a.y * acc_conversion_factor;
    data.acc[2] = _imu.a.z * acc_conversion_factor;

    double gyro_conversion_factor = 8.75 / 1000; // mdps/LSB at 250dps after that devide by 1000 to get dps
    data.gyro[0] = _imu.g.x * gyro_conversion_factor;
    data.gyro[1] = _imu.g.y * gyro_conversion_factor;
    data.gyro[2] = _imu.g.z * gyro_conversion_factor;
}
void Sensor_manager::read_temps(Config &config)
{
    // average values first and take into account temp limits
    if (_inner_temp_probe_initialized)
    {
        data.inner_temp_probe = _inner_temp_probe.readTemperature();
        data.average_inner_temp = data.inner_temp_probe;
    }
    if (_outer_thermistor_initialized)
    {
        data.outter_temp_thermistor = _outer_thermistor.readCelsius();
        data.average_outter_temp = data.outter_temp_thermistor;
    }

    _temp_manager.calculate_heater_power(data.average_inner_temp);
    _temp_manager.set_heater_power();

    data.heater_power = _temp_manager.get_heater_power();
}

void Sensor_manager::read_data(Config &config)
{
    read_gps();
    read_inner_baro(config);
    // read_outter_baro(config);
    read_temps(config);
    read_imu();
    read_ranging(config);
    read_batt_voltage(config);
    position_calculation(config);
    read_time();
}
