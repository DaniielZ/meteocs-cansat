#include "sensors/sensor_manager.h"

float get_altitude(float pressure_hPa, float sea_level_hPa)
{
    float altitude = 44330 * (1.0 - pow(pressure_hPa / sea_level_hPa, 0.1903));
    return altitude;
}

String Sensor_manager::init(Config &config)
{
    String status;
    // GPS
    _gps_serial = &Serial1;
    if (_gps_serial)
    {
        _gps_initialized = true;
    }
    else
    {
        status += " gps error";
    }

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
    _inner_temp_probe_initialized = true;

    // Test temp probe to see if its working
    float test = _inner_temp_probe.readTemperature();
    if (test > 100.00 || test < -100.0 || test == 0.00)
    {
        _inner_temp_probe_initialized = false;
        status += "Inner probe error";
    }

    // Outer temp probe
    analogReadResolution(12);
    _outer_thermistor = NTC_Thermistor(config.THERMISTOR_PIN, config.THERMISTOR_REFERENCE_R, config.THERMISTOR_NOMINAL_R, config.THERMISTOR_NOMINAL_T, config.THERMISTOR_B, 4095);
    _outer_thermistor_initialized = true;

    // TEMP CALCULATOR
    _temp_manager = new Temperature_Manager(config.HEATER_MOSFET, config.DESIRED_HEATER_TEMP);
    
    // TEMP averagers
    _inner_temp_averager = new Time_Averaging_Filter<float>(config.INNER_TEMP_AVERAGE_CAPACITY, config.INNER_TEMP_AVERAGE_TIME);
    _outer_temp_averager = new Time_Averaging_Filter<float>(config.OUTER_TEMP_AVERAGE_CAPACITY, config.OUTER_TEMP_AVERAGE_TIME);

    // RANGING lora
    status += _lora.init(config.LORA2400_MODE, config.ranging_device);

    // Battery averager
    _batt_averager = new Time_Averaging_Filter<float>(config.BAT_AVERAGE_CAPACITY, config.BAT_AVERAGE_TIME);

    return status;
}

void Sensor_manager::read_batt_voltage(Config &config)
{
    pinMode(config.BATT_SENS_PIN, INPUT);
    analogReadResolution(12);
    float adc_reading = analogRead(config.BATT_SENS_PIN);
    data.batt_voltage = (adc_reading / 4095.0) * config.BATT_SENS_CONVERSION_FACTOR;

    _batt_averager->add_data(data.batt_voltage);
    data.average_batt_voltage = _batt_averager->get_averaged_value();
}

void Sensor_manager::position_calculation(Config &config)
{
    Ranging_Wrapper::Position result;
    if (_lora.trilaterate_position(data.ranging_results, config.RANGING_SLAVES, result))
    {
        data.ranging_position = result;
        _last_ranging_pos_time = millis();
    }
    // maybe do more processing
    return;
}

void Sensor_manager::read_ranging(Config &config)
{

    // try to range next slave address
    Ranging_Wrapper::Ranging_Result result = {0, 0};
    bool move_to_next_slave = false;
    if (_lora.master_read(config.RANGING_SLAVES[_slave_index], result, config.RANGING_TIMEOUT))
    {
        // ranging data read and ranging for current slave started
        move_to_next_slave = true;
    }
    // check if something usefull was read from the previous slave
    if (result.distance != 0 && result.time != 0)
    {
        data.ranging_results[_last_slave_index] = result;
        Serial.println(_last_slave_index);
    }

    // move to next slave
    if (move_to_next_slave)
    {
        _last_slave_index = _slave_index;
        int array_length = 3;
        _slave_index++;
        if (_slave_index > array_length - 1)
        {
            // reset index
            _slave_index = 0;
        }
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
            data.gps_satellites = _gps.satellites.value();
            data.gps_time = _gps.time.value();
        }
    }
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
    // read and average values
    if (_inner_temp_probe_initialized)
    {
        data.inner_temp_probe = _inner_temp_probe.readTemperature();
        _inner_temp_averager->add_data(data.inner_temp_probe);
        data.average_inner_temp = _inner_temp_averager->get_averaged_value();
    }
    if (_outer_thermistor_initialized)
    {
        data.outer_temp_thermistor = _outer_thermistor.readCelsius();
        _outer_temp_averager->add_data(data.outer_temp_thermistor);
        data.average_outer_temp = _outer_temp_averager->get_averaged_value();
    }
    if (_heater_enabled)
    {
        _temp_manager->update_heater_power(data.average_inner_temp);

        data.heater_power = _temp_manager->get_heater_power();

        // get pid values
        _temp_manager->get_pid(data.p, data.i, data.d);
        data.target_temp = _temp_manager->get_target_temp();

        // check if should disable heater
        if (data.average_batt_voltage < config.HEATER_CUT_OFF_VOLTAGE)
        {
            set_heater(false);
        }
    }
}

void Sensor_manager::read_data(Config &config)
{
    read_gps();
    read_inner_baro(config);
    read_temps(config);
    read_imu();
    read_ranging(config);
    read_batt_voltage(config);
    position_calculation(config);
    read_time();
}
