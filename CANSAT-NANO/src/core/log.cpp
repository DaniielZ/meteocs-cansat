#include "core/log.h"
// #include "config.h"
// #include "sensors/sensor_manager.h"
#include <SD.h>

void Log::init_lora(Config &config)
{
    // platform specific
    Serial.println("init lora");
    SPI.setMISO(config.LORA_RX);
    SPI.setMOSI(config.LORA_TX);
    SPI.setCS(config.LORA_CS);
    SPI.setSCK(config.LORA_SCK);
    _spi_lora = &SPI;

    _lora.setPins(config.LORA_CS);
    _lora.setSPI(*_spi_lora);
    if (!_lora.begin(config.LORA_FREQUENCY))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    _lora.setTxPower(config.LORA_TXPOWER);
    _lora.setSpreadingFactor(config.LORA_SPREADING);
    _lora.setCodingRate4(config.LORA_CODING_RATE);
    _lora.setSignalBandwidth(config.LORA_SIGNAL_BW);
    Serial.println("LoRa! Running");
    _lora_initialized = true;
}
void Log::init_flash(Config &config)
{
    if (!config.LOG_TO_STORAGE)
    {
        return;
    }
    // initilise flash
    if (SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("FileSystem init success");
    }
    else
    {
        Serial.println("FileSystem init error");
    }

    // determine nr for final path
    int log_file_name_nr = 0;
    // String temp_path = String(config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt");
    while (SD.exists(String(config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt").c_str()))
    {
        log_file_name_nr++;
    }
    _log_file_path_final = config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt";
    // print header
    File file = SD.open(_log_file_path_final.c_str(), FILE_WRITE);
    file.seek(EOF);
    file.println("DATA");
    if (!file)
    {
        Serial.println("Failed opening flash file");
    }
    file.close();
    Serial.println("Final path: " + _log_file_path_final);
    _flash_initialized = true;
}
void Log::init_pcserial(Config &config)
{
    // initilise pc serial
    Serial.begin(config.PC_BAUDRATE);
    if (config.WAIT_PC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }
}
void Log::init(Config &config)
{
    init_pcserial(config);
    init_flash(config);
    init_lora(config);
}
void Log::info(String msg)
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    if (_lora_initialized)
    {
        while (_lora.beginPacket() == 0)
        {
            Serial.print("waiting for lora ... ");
            delay(50);
        }
        _lora.beginPacket();
        _lora.println(msg);
        _lora.endPacket();
    }
    if (_flash_initialized)
    {
        // write to flash
        File file = SD.open(_log_file_path_final.c_str(), FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed opening flash file");
        }
        file.seek(EOF);
        file.println(msg);
        file.close();
    }
}
void Log::data(Sensor_manager::Sensor_data &data, bool log_to_storage)
{
    // unsigned long int start_time = millis();
    // prints data
    Serial.print("DATA: ");
    Serial.print(data.gps_lng, 6);
    Serial.print(",");
    Serial.print(data.gps_lat, 6);
    Serial.print(",");
    Serial.print(data.gps_height);
    Serial.print(",");
    Serial.print(data.gps_sattelites);
    Serial.print(",");
    Serial.print(data.average_value);
    Serial.print(",");
    // Serial.print(data.mag[0]);
    // Serial.print(",");
    // Serial.print(data.mag[1]);
    // Serial.print(",");
    // Serial.print(data.mag[2]);
    // Serial.print(",");
    Serial.print(data.acc[0]);
    Serial.print(",");
    Serial.print(data.acc[1]);
    Serial.print(",");
    Serial.print(data.acc[2]);
    Serial.print(",");
    // Serial.print(data.gyro[0]);
    // Serial.print(",");
    // Serial.print(data.gyro[1]);
    // Serial.print(",");
    // Serial.print(data.gyro[2]);
    // Serial.print(",");
    Serial.print(data.baro_height);
    Serial.print(",");
    Serial.print(data.pressure);
    Serial.print(",");
    Serial.print(data.temperature);
    Serial.print(",");
    Serial.print(data.humidity);
    // Serial.print(",");
    // Serial.print(data.light);
    Serial.print(",");
    Serial.println(data.time);
    // sends data over lora if can be sent

    if (_lora.beginPacket() != 0 && _lora_initialized)
    {
        _lora.beginPacket();
        _lora.print(data.gps_lng, 6);
        _lora.print(",");
        _lora.print(data.gps_lat, 6);
        _lora.print(",");
        _lora.print(data.gps_height);
        _lora.print(",");
        _lora.print(data.gps_sattelites);
        _lora.print(",");
        _lora.print(data.average_value);
        _lora.print(",");
        // _lora.print(data.mag[0]);
        // _lora.print(",");
        // _lora.print(data.mag[1]);
        // _lora.print(",");
        // _lora.print(data.mag[2]);
        // _lora.print(",");
        // _lora.print(data.acc[0]);
        // _lora.print(",");
        // _lora.print(data.acc[1]);
        // _lora.print(",");
        // _lora.print(data.acc[2]);
        // _lora.print(",");
        // _lora.print(data.gyro[0]);
        // _lora.print(",");
        // _lora.print(data.gyro[1]);
        // _lora.print(",");
        // _lora.print(data.gyro[2]);
        // _lora.print(",");
        _lora.print(data.baro_height);
        _lora.print(",");
        _lora.print(data.pressure);
        _lora.print(",");
        _lora.print(data.temperature);
        _lora.print(",");
        _lora.print(data.humidity);
        _lora.print(",");
        // _lora.print(data.light);
        // _lora.print(",");
        _lora.println(data.time);
        _lora.endPacket(true);
    }

    // logs data to flash if apropriate state
    if (log_to_storage && _flash_initialized)
    {
        File file = SD.open(_log_file_path_final.c_str(), FILE_WRITE);
        if (!file)
        {
            Serial.println("File open failed");
        }
        file.seek(EOF);
        file.print(data.gps_lng, 6);
        file.print(",");
        file.print(data.gps_lat, 6);
        file.print(",");
        file.print(data.gps_height);
        file.print(",");
        file.print(data.gps_sattelites);
        file.print(",");
        file.print(data.average_value);
        file.print(",");
        // file.print(data.mag[0]);
        // file.print(",");
        // file.print(data.mag[1]);
        // file.print(",");
        // file.print(data.mag[2]);
        // file.print(",");
        file.print(data.acc[0]);
        file.print(",");
        file.print(data.acc[1]);
        file.print(",");
        file.print(data.acc[2]);
        file.print(",");
        // file.print(data.gyro[0]);
        // file.print(",");
        // file.print(data.gyro[1]);
        // file.print(",");
        // file.print(data.gyro[2]);
        // file.print(",");
        file.print(data.baro_height);
        file.print(",");
        file.print(data.pressure);
        file.print(",");
        file.print(data.temperature);
        file.print(",");
        file.print(data.humidity);
        file.print(",");
        // file.print(data.light);
        // file.print(",");
        file.println(data.time);
        file.close();
    }
}
void Log::read(String &msg)
{
    if (!_lora_initialized)
    {
        return;
    }
    // if anything has been recieved add message to string and return true if msg was read
    int packetSize = _lora.parsePacket();
    if (packetSize)
    {
        while (_lora.available())
        {
            char incoming = (char)_lora.read();
            msg += incoming;
        }
    }
}