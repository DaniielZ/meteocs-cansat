#include "core/log.h"
// #include "config.h"
// #include "sensors/sensor_manager.h"
#include <LittleFS.h>

void Log::init_lora(Config &config)
{
    _spi_lora = new SPIClassRP2040(spi0, config.LORA_RX, config.LORA_CS, config.LORA_SCK, config.LORA_TX);
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
}
void Log::init_flash(Config &config)
{
    // initilise flash
    if (LittleFS.begin())
    {
        Serial.println("FileSystem init success");
    }
    else
    {
        Serial.println("FileSystem init error");
    }

    // determine nr for final path
    while (LittleFS.exists(config.LOG_FILE_NAME_BASE_PATH + String(_log_file_name_nr) + ".txt"))
    {
        _log_file_name_nr++;
    }
    _log_file_path_final = config.LOG_FILE_NAME_BASE_PATH + String(_log_file_name_nr) + ".txt";
    // print header
    File file = LittleFS.open(_log_file_path_final, "a+");
    file.println("");
}
void Log::init(Config &config)
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

    init_flash(config);
    init_lora(config);
}
void Log::info(String msg)
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    _lora.beginPacket();
    _lora.println(msg);
    _lora.endPacket();
    // write to flash
    File file = LittleFS.open(_log_file_path_final, "a+");
    file.println(msg);
    file.close();
}
void Log::data(Sensor_manager::Sensor_data &data, bool log_to_storage)
{
    // prints data
    Serial.print("DATA: ");
    Serial.print(data.gps_lng);
    Serial.print(",");
    Serial.print(data.gps_lat);
    Serial.print(",");
    Serial.print(data.gps_height);
    Serial.print(",");
    Serial.print(data.gps_sattelites);
    Serial.print(",");
    Serial.print(data.mag[0]);
    Serial.print(",");
    Serial.print(data.mag[1]);
    Serial.print(",");
    Serial.print(data.mag[2]);
    Serial.print(",");
    Serial.print(data.acc[0]);
    Serial.print(",");
    Serial.print(data.acc[1]);
    Serial.print(",");
    Serial.print(data.acc[2]);
    Serial.print(",");
    Serial.print(data.baro_height);
    Serial.print(",");
    Serial.print(data.pressure);
    Serial.print(",");
    Serial.print(data.temperature);
    Serial.print(",");
    Serial.print(data.humidity);
    Serial.print(",");
    Serial.print(data.light);
    Serial.print(",");
    Serial.println(data.time);

    // sends data over lora if can be sent
    if (_lora.beginPacket() != 0)
    {
        _lora.print(data.gps_lng);
        _lora.print(",");
        _lora.print(data.gps_lat);
        _lora.print(",");
        _lora.print(data.gps_height);
        _lora.print(",");
        _lora.print(data.gps_sattelites);
        _lora.print(",");
        _lora.print(data.mag[0]);
        _lora.print(",");
        _lora.print(data.mag[1]);
        _lora.print(",");
        _lora.print(data.mag[2]);
        _lora.print(",");
        _lora.print(data.acc[0]);
        _lora.print(",");
        _lora.print(data.acc[1]);
        _lora.print(",");
        _lora.print(data.acc[2]);
        _lora.print(",");
        _lora.print(data.baro_height);
        _lora.print(",");
        _lora.print(data.pressure);
        _lora.print(",");
        _lora.print(data.temperature);
        _lora.print(",");
        _lora.print(data.humidity);
        _lora.print(",");
        _lora.print(data.light);
        _lora.print(",");
        _lora.println(data.time);
    }
    // logs data to flash if apropriate state
    if (log_to_storage)
    {
        File file = LittleFS.open(_log_file_path_final, "a+");
        file.print(data.gps_lng);
        file.print(",");
        file.print(data.gps_lat);
        file.print(",");
        file.print(data.gps_height);
        file.print(",");
        file.print(data.gps_sattelites);
        file.print(",");
        file.print(data.mag[0]);
        file.print(",");
        file.print(data.mag[1]);
        file.print(",");
        file.print(data.mag[2]);
        file.print(",");
        file.print(data.acc[0]);
        file.print(",");
        file.print(data.acc[1]);
        file.print(",");
        file.print(data.acc[2]);
        file.print(",");
        file.print(data.baro_height);
        file.print(",");
        file.print(data.pressure);
        file.print(",");
        file.print(data.temperature);
        file.print(",");
        file.print(data.humidity);
        file.print(",");
        file.print(data.light);
        file.print(",");
        file.println(data.time);
        file.close();
    }
}
bool Log::read(String &msg)
{
    // if anything has been recieved add message to string and return true if msg was read
    int packetSize = _lora.parsePacket();

    if (packetSize)
    {
        while (_lora.available())
        {
            char incoming = (char)_lora.read();
            msg += incoming;
        }
        return true;
    }
    else
    {
        return false;
    }
}