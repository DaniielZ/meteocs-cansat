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
        Serial.println("Config set");
    }
    else
    {
        Serial.println("Config not set");
    }
    // Initialize flash
    if (_flash->begin())
    {
        Serial.println("FileSystem init success");
    }
    else
    {
        Serial.println("FileSystem init error");
        return;
    }

    // Determine file name number for final path
    int log_file_name_nr = 0;
    while (_flash->exists(config.TELEMETRY_LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".csv"))
    {
        log_file_name_nr++;
    }

    _telemetry_log_file_path_final = config.TELEMETRY_LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".csv";
    _info_log_file_path_final = config.INFO_LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".csv";
    _error_log_file_path_final = config.ERROR_LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".csv";
    
    // Save file header
    File telemetry_file = _flash->open(_telemetry_log_file_path_final, "a+");
    File error_file = _flash->open(_error_log_file_path_final, "a+");
    File info_file = _flash->open(_info_log_file_path_final, "a+");
    if (!telemetry_file)
    {
        Serial.println("Failed opening telemetry file");
        return;
    }
    if (!error_file)
    {
        Serial.println("Failed opening error file");
        return;
    }

    telemetry_file.println(config.TELEMETRY_HEADER);
    telemetry_file.close();
    
    info_file.println(config.INFO_HEADER);
    info_file.close();

    error_file.println(config.ERROR_HEADER);
    error_file.close();

    // Print info about storage to PC
    Serial.println("Final telemetry path: " + _telemetry_log_file_path_final);
    Serial.println("Final info path: " + _info_log_file_path_final);
    Serial.println("Final error path: " + _error_log_file_path_final);

    //FSInfo64 fsinfo;
    //_flash->info64(fsinfo);
    //Serial.println("Current size:" + String((unsigned long)fsinfo.usedBytes / 1024) + "/" + String((unsigned long)fsinfo.totalBytes / 1024));

    _flash_initialized = true;
}

// Call flash and LoRa init
void Log::init(Config &config)
{
    // Init SD card
    init_flash(config);

    // Init LoRa
    String status = _com_lora.init(true, config.com_config);

    // Print status
    status = "INIT STATUS: Flash ready?:" + String(_flash_initialized) + " | " + "Lora ready?:" + String(_com_lora.get_init_status());
    send_info(status, config);
}

void Log::receive_main_lora(String &msg, float &rssi, float &snr, Config &config)
{
    // Check if LoRa is initialized
    if (!_com_lora.get_init_status())
    {
        return;
    }

    // Get data from LoRa
    bool state = _com_lora.receive(msg, rssi, snr);

    // Finish receive if all good
    if (state != RADIOLIB_ERR_NONE)
    {
        return;
    }
    // log_error_msg_to_flash("Receiving data from main LoRa failed with error code: " + String(state));
    msg = "";
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

void Log::write_to_file(String msg, String file_name)
{
    // Write info to SD card
    if (_flash_initialized)
    {
        // write to flash
        File file = _flash->open(file_name, "a+");
        if (!file)
        {
            Serial.println("Failed opening file: " + String(file_name));
            return;
        }
        file.println(msg);
        file.close();
    }
}

void Log::send_info(String msg, Config &config)
{
    // Prints message to serial
    Serial.println("! " + msg);

    // Sends message over LoRa
    int state = send_main_lora(msg, config);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("Transmit error: " + String(state));
    }
    
    // Log data to info file
    msg = String(millis()) + "," + msg; 
    write_to_file(msg, _info_log_file_path_final);
} 

void Log::send_error(String msg, Config &config)
{
    // Prints message to serial
    Serial.println("!!! " + msg);

    // Sends message over LoRa
    int state = send_main_lora(msg, config);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("Transmit error: " + String(state));
    }

    // Log data to error file
    msg = String(millis()) + "," + msg; 
    write_to_file(msg, _error_log_file_path_final);
}

void Log::update_data_packet(Sensor_manager::Sensor_data &data, String &result_sent, String &result_log)
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

void Log::transmit_data(Config &config)
{
    // Serial.println("size of packet:" + String(packet.length()));
    int state = send_main_lora(_sendable_packet, config);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("Transmit error: " + String(state));
    }
}

void Log::log_data_to_flash()
{
    write_to_file(_loggable_packet, _telemetry_log_file_path_final);
}

void Log::log_data_to_pc()
{
    Serial.print("/*");
    // Serial.print(sendable_packet);
    Serial.print(_loggable_packet);
    Serial.println("*/");
}

void Log::log_info_msg_to_flash(String msg)
{
    write_to_file(msg, _info_log_file_path_final);
}

void Log::log_error_msg_to_flash(String msg)
{
    write_to_file(msg, _error_log_file_path_final);
}