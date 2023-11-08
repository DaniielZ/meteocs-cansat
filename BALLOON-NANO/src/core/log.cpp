#include "core/log.h"
#include <SDFS.h>

// Format SD card
bool Log::format_storage(Config &config)
{
    bool result = _flash->format();
    init_flash(config);
    return result;
}

// Initialize SD card
void Log::init_flash(Config &config)
{
    // Check if SD card should even be initialized
    if (!config.LOG_TO_STORAGE)
    {
        return;
    }

    // Config
    _flash = config.FILE_SYSTEM;
    SDFSConfig sd_config;
    sd_config.setCSPin(config.SD_CARD_CS);
    sd_config.setSPI(*config.SD_CARD_SPI);

    if (_flash->setConfig(sd_config))
    {
        Serial.println("config set");
    }
    else
    {
        Serial.println("config not set");
    }
    // Initialize flash
    if (_flash->begin())
    {
        Serial.println("FileSystem init success");
    }
    else
    {
        Serial.println("FileSystem init error");
    }

    // Determine file name number for final path
    int log_file_name_nr = 0;
    while (_flash->exists(config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt"))
    {
        log_file_name_nr++;
    }
    _log_file_path_final = config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt";
    // Save file header
    File file = _flash->open(_log_file_path_final, "a+");
    if (!file)
    {
        Serial.println("Failed opening flash file");
        return;
    }

    // ADD PROPER HEADER!!!!!!!!!!!!!!!!!!!!!!!
    file.println("DATA");
    file.close();

    // Print info about storage to PC
    Serial.println("Final path: " + _log_file_path_final);

    FSInfo64 fsinfo;
    _flash->info64(fsinfo);
    Serial.println("Current size:" + String((unsigned long)fsinfo.usedBytes / 1024) + "/" + String((unsigned long)fsinfo.totalBytes / 1024));

    _flash_initialized = true;
}

void Log::init(Config &config)
{
    // Init SD card
    init_flash(config);

    // Init LoRa
    String status = _com_lora.init(true, config.com_config);

    // Print status
    String status = " INIT STATUS: Flash ready?:" + String(_flash_initialized) + " | " + "Lora ready?:" + String(_com_lora.get_init_status());
    info(status, config);
}

bool Log::send_main_lora(String msg, Config &config)
{
    if (!_com_lora.get_init_status())
    {
        return false;
    }

    while (_com_lora.send(msg) == false)
    {
        delay(1);
    }

    return true;
}

void Log::info(String msg, Config &config)
{
    // Prints message to serial
    msg = "!" + msg;
    Serial.println(msg);

    // Sends message over lora
    // Serial.println("size of packet:" + String(msg.length()));
    int state = send_main_lora(msg, config);
    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.println("Transmit error: " + String(state));
    }

    // Write info to SD card
    if (_flash_initialized)
    {
        // write to flash
        File file = _flash->open(_log_file_path_final, "a+");
        if (!file)
        {
            Serial.println("Failed opening flash file");
            return;
        }
        file.println(msg);
        file.close();
    }
}

void Log::data(Sensor_manager::Sensor_data &data, bool log_to_storage, bool transmit, Config &config)
{
    String sendable_packet;
    String loggable_packet;

    data_to_packet(data, sendable_packet, loggable_packet);

    // sends data over lora if can be sent
    if (transmit)
    {
        // Serial.println("size of packet:" + String(packet.length()));
        int state = send_main_lora(sendable_packet, config);
        if (state != RADIOLIB_ERR_NONE)
        {
            Serial.println("Transmit error: " + String(state));
        }
    }

    Serial.print("/*");
    // Serial.print(sendable_packet);
    Serial.println(loggable_packet);
    Serial.println("*/");

    // logs data to flash if appropriate state
    if (log_to_storage && _flash_initialized)
    {
        File file = _flash->open(_log_file_path_final, "a+");
        if (!file)
        {
            Serial.println("File open failed");
            return;
        }

        file.println(loggable_packet);
        file.close();
    }
}

void Log::data_to_packet(Sensor_manager::Sensor_data &data, String &result_sent, String &result_log)
{
    String packet;
    // GPS
    packet += String(data.gps_lat, 6); // 1
    packet += ",";
    packet += String(data.gps_lng, 6); // 2
    packet += ",";
    packet += String(data.gps_height, 2); // 3
    packet += ",";
    packet += String(data.gps_satellites); // 4
    packet += ",";
    // Ranging
    packet += String(data.ranging_results[0].distance, 2); // 5
    packet += ",";
    packet += String(data.ranging_results[1].distance, 2); // 6
    packet += ",";
    packet += String(data.ranging_results[2].distance, 2); // 7
    packet += ",";
    packet += String(data.times_since_last_ranging_result[0]); // 8
    packet += ",";
    packet += String(data.times_since_last_ranging_result[1]); // 9
    packet += ",";
    packet += String(data.times_since_last_ranging_result[2]); // 10
    packet += ",";
    packet += String(data.ranging_position.lat, 6); // 11
    packet += ",";
    packet += String(data.ranging_position.lng, 6); // 12
    packet += ",";
    packet += String(data.ranging_position.height, 2); // 13
    packet += ",";
    packet += String(data.time_since_last_ranging_pos); // 14
    packet += ",";
    // Baro
    packet += String(data.inner_baro_pressure, 3); // 15
    packet += ",";
    // Temperatures
    packet += String(data.average_inner_temp, 2); // 16
    packet += ",";
    packet += String(data.average_outer_temp, 2); // 17
    packet += ",";
    // Heater
    packet += String(data.heater_power); // 18
    packet += ",";
    // Accelerometer
    packet += String(data.acc[0], 4); // 19
    packet += ",";
    packet += String(data.acc[1], 4); // 20
    packet += ",";
    packet += String(data.acc[2], 4); // 21
    packet += ",";
    // Misc
    packet += String(data.time_since_last_gps); // 22
    packet += ",";
    packet += String(data.time); // 23
    packet += ",";
    packet += String(data.average_batt_voltage, 2); // 24

    result_sent = packet;

    packet += ",";
    packet += String(data.gps_time); // 25
    packet += ",";
    // Gyro
    packet += String(data.gyro[0], 2); // 26
    packet += ",";
    packet += String(data.gyro[1], 2); // 27
    packet += ",";
    packet += String(data.gyro[2], 2); // 28
    packet += ",";
    // Temperatures
    packet += String(data.outer_temp_thermistor, 2); // 29
    packet += ",";
    packet += String(data.inner_baro_temp, 2); // 30
    packet += ",";
    packet += String(data.inner_temp_probe, 2); // 31
    packet += ",";
    // Battery voltage
    packet += String(data.batt_voltage, 2); // 32
    packet += ",";
    // PID
    packet += String(data.p, 4); // 33
    packet += ",";
    packet += String(data.i, 4); // 34
    packet += ",";
    packet += String(data.d, 4); // 35
    packet += ",";
    packet += String(data.target_temp, 1); // 36
    //packet += ",";
    // Ranging
    /*
    packet += String(data.ranging_results[0].time); // 37
    packet += ",";
    packet += String(data.ranging_results[0].rssi, 2); // 38
    packet += ",";
    packet += String(data.ranging_results[0].snr, 2); // 39
    packet += ",";
    packet += String(data.ranging_results[0].f_error, 2); // 40
    packet += ",";
    packet += String(data.ranging_results[1].time); // 41
    packet += ",";
    packet += String(data.ranging_results[1].rssi, 2); // 42
    packet += ",";
    packet += String(data.ranging_results[1].snr, 2); // 43
    packet += ",";
    packet += String(data.ranging_results[1].f_error, 2); // 44
    packet += ",";
    packet += String(data.ranging_results[2].time); // 45
    packet += ",";
    packet += String(data.ranging_results[2].rssi, 2); // 46
    packet += ",";
    packet += String(data.ranging_results[2].snr, 2); // 47
    packet += ",";
    packet += String(data.ranging_results[2].f_error, 2); // 48
    */
    result_log = packet;
}

void Log::read(String &msg)
{
    if (_com_lora.get_init_status())
    {
        return;
    }

    float rssi;
    float snr;

    int state = _com_lora.receive(msg, rssi, snr);

    if (state == RADIOLIB_ERR_NONE)
    {
        return;
    }

    msg = "";
}