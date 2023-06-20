#include "sensors/sensor_manager.h"
volatile bool sx1280_lora_ranging = false; // yeah sadly wasnt able to move it into the class
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

    // WIRE1
    Wire1.setSCL(config.WIRE1_SCL);
    Wire1.setSDA(config.WIRE1_SDA);
    // GPS UART0
    _gps_serial = &Serial1;
    _gps_serial->setFIFOSize(256); // once had a prblem of not reading serial properly but this seemed to fix it
    _gps_serial->begin(config.GPS_BAUDRATE);
    _gps_initialized = true;

    // BARO WIRE1
    _baro = MS5611(config.MS5611_ADDRESS);
    if (!_baro.begin(&Wire1))
    {
        status += " Baro error";
    }
    else
    {
        _baro_initialized = true;
    }
    // HUMIDITY WIRE1
    if (!_humidity.begin(&Wire1))
    {
        status += " Humidity error";
    }
    else
    {
        _humidity_initialized = true;
    }

    // IMU WIRE1
    _imu = Adafruit_BNO055(55, config.BNO055_ADDRESS, &Wire1);
    if (!_imu.begin())
    {
        status += " IMU error";
    }
    else
    {
        _imu_initialized = true;
    }
    // mybe need to set axis max range TODO

    // RANGING lora
    Config::Lora_device &lora_cfg = config.LORA2400;
    lora_cfg.SPI->setRX(lora_cfg.RX);
    lora_cfg.SPI->setTX(lora_cfg.TX);
    lora_cfg.SPI->setCS(lora_cfg.CS);
    lora_cfg.SPI->setSCK(lora_cfg.SCK);
    *_lora = new Module(lora_cfg.CS, lora_cfg.DIO1, lora_cfg.RESET, lora_cfg.DIO1, *lora_cfg.SPI); // busy pin doesnt coutn
    int state = _lora->begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("SX1280 lora failed state: ");
        Serial.println(state);
    }
    else
    {
        // setting paramaters
        _lora->setOutputPower(lora_cfg.TXPOWER);
        _lora->setSpreadingFactor(lora_cfg.SPREADING);
        _lora->setCodingRate(lora_cfg.CODING_RATE);
        _lora->setBandwidth(lora_cfg.SIGNAL_BW);
        _lora->setSyncWord(lora_cfg.SYNC_WORD);
        Serial.println("SX1280 LoRa! Running");
        _lora_initialized = true;

        //
        _lora->startRanging(false, config.RANGING_SLAVE_ADDRESS);
        sx1280_lora_ranging = true;
        // need to setup interupt
        _lora->setDio1Action(sx1280_ranging_end);
        // need to wait for irq to be high
    }

    return status;
}
void Sensor_manager::enable_ranging(Config &config)
{
    if (!sx1280_lora_ranging)
    {
        _lora->startRanging(false, config.RANGING_SLAVE_ADDRESS);
        sx1280_lora_ranging = true;
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
    data.temperature = _baro.getTemperature();
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
    if (!_imu_initialized)
    {
        Serial.println("IMU not init.. returning");
        return;
    }

    sensors_event_t gyroData, accData;
    _imu.getEvent(&gyroData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    _imu.getEvent(&accData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    data.acc[0] = accData.acceleration.x;
    data.acc[1] = accData.acceleration.y;
    data.acc[2] = accData.acceleration.z;

    data.gyro[0] = accData.gyro.x;
    data.gyro[1] = accData.gyro.x;
    data.gyro[2] = accData.gyro.x;
}
void Sensor_manager::read_data(Config &config)
{
    read_gps();
    read_baro(config);
    read_humidity();
    read_imu();
    read_time();
}
