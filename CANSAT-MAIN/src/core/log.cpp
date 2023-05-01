#include "core/log.h"

void Log::init_lora(Cansat &cansat)
{
    _spi_lora = SPIClassRP2040(spi0, cansat.config.LORA_RX, cansat.config.LORA_CS, cansat.config.LORA_SCK, cansat.config.LORA_TX);
    _lora.setPins(cansat.config.LORA_CS);
    _lora.setSPI(_spi_lora);
    if (!_lora.begin(cansat.config.LORA_FREQUENCY))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    // setting paramaters
    _lora.setTxPower(cansat.config.LORA_TXPOWER);
    _lora.setSpreadingFactor(cansat.config.LORA_SPREADING);
    _lora.setCodingRate4(cansat.config.LORA_CODING_RATE);
    _lora.setSignalBandwidth(cansat.config.LORA_SIGNAL_BW);
    Serial.println("LoRa! Running");
}
void Log::init_flash(Cansat &cansat)
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
    while (LittleFS.exists(cansat.config.LOG_FILE_NAME_BASE_PATH + String(_log_file_name_nr) + ".txt"))
    {
        _log_file_name_nr++;
    }
    _log_file_path_final = cansat.config.LOG_FILE_NAME_BASE_PATH + String(_log_file_name_nr) + ".txt";
    // print header
    File file = LittleFS.open(_log_file_path_final, "a+");
    file.println("");
}
void Log::init(Cansat &cansat)
{
    // initilise pc serial
    Serial.begin(cansat.config.PC_BAUDRATE);
    if (cansat.config.WAIT_PC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }

    init_flash(cansat);
    init_lora(cansat);
}
void Log::info(char msg[])
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    _lora.beginPacket();
    _lora.println(msg);
    _lora.endPacket();
    // write to flash
    File file = LittleFS.open(_log_file_path_final, "a+");
    file.println(String(msg));
    file.close();
}
void Log::error(Cansat &cansat, char msg[])
{
    info(msg);
    // play error sound
    cansat.sound.error(cansat);
}
void Log::data(Cansat &cansat)
{
    // prints data
    Serial.print("DATA: ");
    Serial.print(cansat.sensors.data.gps_lng);
    Serial.print(",");
    Serial.print(cansat.sensors.data.gps_lat);
    Serial.print(",");
    Serial.print(cansat.sensors.data.gps_height);
    Serial.print(",");
    Serial.print(cansat.sensors.data.gps_sattelites);
    Serial.print(",");
    Serial.print(cansat.sensors.data.mag[0]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.mag[1]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.mag[2]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.acc[0]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.acc[1]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.acc[2]);
    Serial.print(",");
    Serial.print(cansat.sensors.data.baro_height);
    Serial.print(",");
    Serial.print(cansat.sensors.data.pressure);
    Serial.print(",");
    Serial.print(cansat.sensors.data.temperature);
    Serial.print(",");
    Serial.print(cansat.sensors.data.humidity);
    Serial.print(",");
    Serial.print(cansat.sensors.data.light);
    Serial.print(",");
    Serial.println(cansat.sensors.data.time);

    // sends data over lora if can be sent
    if (_lora.beginPacket() != 0)
    {
        _lora.print(cansat.sensors.data.gps_lng);
        _lora.print(",");
        _lora.print(cansat.sensors.data.gps_lat);
        _lora.print(",");
        _lora.print(cansat.sensors.data.gps_height);
        _lora.print(",");
        _lora.print(cansat.sensors.data.gps_sattelites);
        _lora.print(",");
        _lora.print(cansat.sensors.data.mag[0]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.mag[1]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.mag[2]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.acc[0]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.acc[1]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.acc[2]);
        _lora.print(",");
        _lora.print(cansat.sensors.data.baro_height);
        _lora.print(",");
        _lora.print(cansat.sensors.data.pressure);
        _lora.print(",");
        _lora.print(cansat.sensors.data.temperature);
        _lora.print(",");
        _lora.print(cansat.sensors.data.humidity);
        _lora.print(",");
        _lora.print(cansat.sensors.data.light);
        _lora.print(",");
        _lora.println(cansat.sensors.data.time);
    }
    // logs data to flash if apropriate state
    if (cansat.current_state != cansat.LANDED)
    {
        File file = LittleFS.open(_log_file_path_final, "a+");
        file.print(cansat.sensors.data.gps_lng);
        file.print(",");
        file.print(cansat.sensors.data.gps_lat);
        file.print(",");
        file.print(cansat.sensors.data.gps_height);
        file.print(",");
        file.print(cansat.sensors.data.gps_sattelites);
        file.print(",");
        file.print(cansat.sensors.data.mag[0]);
        file.print(",");
        file.print(cansat.sensors.data.mag[1]);
        file.print(",");
        file.print(cansat.sensors.data.mag[2]);
        file.print(",");
        file.print(cansat.sensors.data.acc[0]);
        file.print(",");
        file.print(cansat.sensors.data.acc[1]);
        file.print(",");
        file.print(cansat.sensors.data.acc[2]);
        file.print(",");
        file.print(cansat.sensors.data.baro_height);
        file.print(",");
        file.print(cansat.sensors.data.pressure);
        file.print(",");
        file.print(cansat.sensors.data.temperature);
        file.print(",");
        file.print(cansat.sensors.data.humidity);
        file.print(",");
        file.print(cansat.sensors.data.light);
        file.print(",");
        file.println(cansat.sensors.data.time);
        file.close();
    }
}
bool Log::read(Cansat &cansat, String &msg)
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