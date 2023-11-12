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

// Call flash, eeprom and LoRa init
void Log::init(Config &config)
{
    // Init SD card
    init_flash(config);

    // Init eeprom
    EEPROM.begin(512);

    // Init LoRa
    String status = _com_lora.init(true, config.com_config);

    // Print status
    status = "INIT STATUS: Flash ready?:" + String(_flash_initialized) + " | " + "Lora ready?:" + String(_com_lora.get_init_status());
    send_info(status, config);
}

// Checks if LoRa has received any messages. Sets the message to the received one, or to empty string otherwise
void Log::receive_main_lora(String &msg, float &rssi, float &snr, Config &config)
{
    // Check if LoRa is initialized
    if (!_com_lora.get_init_status())
    {
        return;
    }

    // Get data from LoRa
    bool message_received = _com_lora.receive(msg, rssi, snr);

    // Finish receive if all good
    if (message_received)
    {
        return;
    }

    // Default message
    msg = "";
}

// Sends the provided message using LoRa
bool Log::send_main_lora(String msg, Config &config)
{
    // Check if LoRa is initialized
    if (!_com_lora.get_init_status())
    {
        return false;
    }

    // Wait while transmission is happening
    while (_com_lora.send(msg) == false)
    {
        delay(1);
    }

    return true;
}

// Writes a given message to a file on the SD card
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

// Sends a message over LoRa and logs the message to the info file 
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

// Sends a message over LoRa and logs the message to the error file 
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

// Send telemetry data packet over LoRa
void Log::transmit_data(Config &config)
{
    // Serial.println("size of packet:" + String(packet.length()));
    int state = send_main_lora(_sendable_packet, config);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("Transmit error: " + String(state));
    }
}

// Logs the full data packet to SD card
void Log::log_data_to_flash()
{
    write_to_file(_loggable_packet, _telemetry_log_file_path_final);
}

// Sends the full data packet to Serial port
void Log::log_data_to_pc()
{
    Serial.print("/*");
    // Serial.print(sendable_packet);
    Serial.print(_loggable_packet);
    Serial.println("*/");
}

// Logs the provided info message to info file on SD card
void Log::log_info_msg_to_flash(String msg)
{
    write_to_file(msg, _info_log_file_path_final);
}

// Logs the provided error message to error file on SD card
void Log::log_error_msg_to_flash(String msg)
{
    write_to_file(msg, _error_log_file_path_final);
}

// Read from EEPROM
bool Log::read_eeprom(int address)
{
    bool value;
    EEPROM.get(address, value);
    return value;
}

// Write to eeprom
void Log::write_eeprom(int address, int value)
{
    EEPROM.put(address, value);
}