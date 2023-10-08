#include "sensors/sensor_manager.h"

volatile bool sx1280_lora_ranging = false; // can't be moved inside class. Shows if lora is actively doing ranging
void sx1280_ranging_end(void)
{
    sx1280_lora_ranging = false;
}

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
    _baro = MS5611(config.MS5611_ADDRESS);
    if (!_baro.begin(&Wire))
    {
        status += " Baro error";
    }
    else
    {
        _baro_initialized = true;
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
    _imu = Adafruit_BNO055(55, config.BNO055_ADDRESS, &Wire);
    if (!_imu.begin())
    {
        status += " IMU error";
    }
    else
    {
        _imu_initialized = true;
    }
    // INNER TEMP WIRE1
    _inner_temp = ClosedCube::Sensor::STS35(config.STS35_ADDRESS, &Wire);
    // OUTTER TEMP ANALOG

    // mybe need to set axis max range TODO
    // RANGING lora
    Config::Lora_device lora_cfg = config.LORA2400;

    int state = _lora.begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        status += " SX1280 error (" + String(state) + ")";
    }
    else
    {
        // setting paramaters
        _lora.setOutputPower(lora_cfg.TXPOWER);
        _lora.setSpreadingFactor(lora_cfg.SPREADING);
        _lora.setCodingRate(lora_cfg.CODING_RATE);
        _lora.setBandwidth(lora_cfg.SIGNAL_BW);
        _lora.setSyncWord(lora_cfg.SYNC_WORD);
        _lora.setFrequency(lora_cfg.FREQUENCY);
        _lora_initialized = true;
    }

    return status;
}
void Sensor_manager::position_calculation(Config &config)
{

    // set global space
    float global_bs_pos_lat[3];
    float global_bs_pos_lng[3];
    float global_bs_pos_height[3];
    for (int i = 0; i < 3; i++)
    {
        global_bs_pos_lat[i] = config.RANGING_SLAVES[i].lat;
        global_bs_pos_lng[i] = config.RANGING_SLAVES[i].lng;
        global_bs_pos_height[i] = config.RANGING_SLAVES[i].height;
    }

    // global space to local space
    float local_bs_pos_lat[3];
    float local_bs_pos_lng[3];
    float local_bs_pos_height[3];

    // calculate pos in local space

    // local space to global space

    // set results
    data.ranging_height = 0;
    data.ranging_lat = 0;
    data.ranging_lng = 0;
}

void Sensor_manager::read_ranging(Config &config)
{
    if (sx1280_lora_ranging)
    {
        // check if should timeout
        if (millis() >= _ranging_start_time + config.RANGING_TIMEOUT)
        {
            sx1280_lora_ranging = false;
            _lora_range_state = RADIOLIB_ERR_RANGING_TIMEOUT;
            _lora.clearDio1Action();
            _lora.finishTransmit();
        }
        return;
    }
    if (!sx1280_lora_ranging)
    {

        // if available read result
        if (_lora_range_state == RADIOLIB_ERR_NONE)
        {
            data.ranging_result[_lora_slave_index].distance = _lora.getRangingResult();
            data.ranging_result[_lora_slave_index].time = millis();
        }

        // clean up
        _lora.clearDio1Action();
        _lora.finishTransmit();
        _lora_range_state = -1;

        // increment next address
        int array_length = sizeof(config.RANGING_SLAVES) / sizeof(Config::Ranging_slave);
        if (_lora_slave_index >= array_length - 1)
        {
            // reset index
            _lora_slave_index = 0;
        }
        else
        {
            _lora_slave_index++;
        }

        // start ranging but first reset lora
        _lora.reset();
        int state = _lora.begin();
        if (state != RADIOLIB_ERR_NONE)
        {
            Serial.println("Ranging not init.. returning");
            return;
        }
        else
        {
            // setting paramaters
            _lora.setOutputPower(config.LORA2400.TXPOWER);
            _lora.setSpreadingFactor(config.LORA2400.SPREADING);
            _lora.setCodingRate(config.LORA2400.CODING_RATE);
            _lora.setBandwidth(config.LORA2400.SIGNAL_BW);
            _lora.setSyncWord(config.LORA2400.SYNC_WORD);
            _lora.setFrequency(config.LORA2400.FREQUENCY);
            _lora_initialized = true;
        }

        // setup interrupt
        _lora.setDio1Action(sx1280_ranging_end);
        sx1280_lora_ranging = true;
        _lora_range_state = _lora.startRanging(true, config.RANGING_SLAVES[_lora_slave_index].address);

        if (_lora_range_state != RADIOLIB_ERR_NONE)
        {
            Serial.println("Ranging error");
        }
        _ranging_start_time = millis();
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
    data.outer_tempeature = _baro.getTemperature();
}
void Sensor_manager::read_humidity()
{
    if (!_humidity_initialized)
    {
        Serial.println("Humidity not init.. returning");
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
}
void Sensor_manager::read_imu()
{
    // if (!_imu_initialized)
    // {
    //     // Serial.println("IMU not init.. returning");
    //     return;
    // }

    // sensors_event_t gyroData, accData;
    // _imu.getEvent(&gyroData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    // _imu.getEvent(&accData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    // data.acc[0] = accData.acceleration.x;
    // data.acc[1] = accData.acceleration.y;
    // data.acc[2] = accData.acceleration.z;

    // data.gyro[0] = accData.gyro.x;
    // data.gyro[1] = accData.gyro.x;
    // data.gyro[2] = accData.gyro.x;
}
void Sensor_manager::read_inner_temperature()
{
    _inner_temp._inner_temp.readTemperature() l;
}
void Sensor_manager::read_outter_tempeature(Config &config)
{
}
void Sensor_manager::read_data(Config &config)
{

    read_gps();
    read_baro(config);
    read_humidity();
    read_imu();
    read_ranging(config);
    position_calculation(config);
    read_time();
}
