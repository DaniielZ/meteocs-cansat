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

    // WIRE0
    Wire.setSCL(config.LIS2MDL_SCL);
    Wire.setSDA(config.LIS2MDL_SDA);
    // WIRE1
    Wire1.setSCL(config.SHTC3_SCL);
    Wire1.setSDA(config.SHTC3_SDA);
    // GPS UART0
    _gps_serial = &Serial1;
    _gps_serial->setFIFOSize(256);
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

    // GYRO WIRE0
    if (_gyro.I3G4250D_Init(0x0F, 0x00, 0x00, 0x00, 0x00, I3G4250D_SCALE_2000) != 0)
    {
        status += "Gyro error";
        Serial.println("Gyro turned off");
    }
    else
    {
        uint8_t gyro_id = 0;
        _gyro.readRegister(0x0F, &gyro_id, 1);

        if (gyro_id != 0xD3)
        {
            status += "Gyro ID error";
            Serial.println("Gyro turned off");
        }
        else
        {
            _gyro_initialized = true;
        }
    }
    // ACC WIRE1
    _acc = new H3LIS100(123);
    if (!_acc->begin(0x19, &Wire1))
    {
        status += "Acc error";
        Serial.println("Acc turned off");
    }
    else
    {
        _acc_initialized = true;
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
            last_gps_packet_time = millis();
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
    if (!_acc_initialized)
    {
        return;
    }
    sensors_event_t event;
    _acc->getEvent(&event);
    data.acc[0] = event.acceleration.x;
    data.acc[1] = event.acceleration.y;
    data.acc[2] = event.acceleration.z;
}
void Sensor_manager::read_gyro()
{
    if (!_gyro_initialized)
    {
        return;
    }
    I3G4250D_DataScaled gyro_data = {0};
    gyro_data = _gyro.I3G4250D_GetScaledData();
    data.gyro[0] = gyro_data.x;
    data.gyro[1] = gyro_data.y;
    data.gyro[2] = gyro_data.z;
}
void Sensor_manager::read_light(Config &config)
{
    analogReadResolution(12);
    data.light = analogRead(config.PHOTO_ADC);
}
void Sensor_manager::read_time()
{
    data.time = millis();
    data.time_since_last_gps = data.time - last_gps_packet_time;
}

void Sensor_manager::read_data(Config &config)
{
    // data = {0, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, 0, 0};
    read_gps();
    read_magneto();
    read_baro(config);
    read_humidity();
    read_light(config);
    read_acc();
    read_gyro();
    read_time();
}
