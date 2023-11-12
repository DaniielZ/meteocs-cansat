#pragma once
#include <SPI.h>
#include <RadioLib.h>
#include <EEPROM.h>
#include <Arduino.h>
#include "config.h"

class Log
{
    RFM_Wrapper _com_lora;
    FS *_flash;
    String _telemetry_log_file_path_final;
    String _info_log_file_path_final;
    String _error_log_file_path_final;

    bool _flash_initialized = false;
    void init_flash(Config &config);

public:
    String _loggable_packet;
    String _sendable_packet;

    void init(Config &config);
    void write_to_file(String msg, String file_name);
    void send_info(String msg, Config &config);
    void send_error(String msg, Config &config);
    
    bool read_eeprom(int address);
    void write_eeprom(int address, int value);

    void transmit_data(Config &config);
    void log_data_to_flash();
    void log_info_msg_to_flash(String msg);
    void log_error_msg_to_flash(String msg);
    void log_data_to_pc();
    
    void receive_main_lora(String &msg, float &rssi, float &snr, Config &config);
    bool format_storage(Config &config);
    bool send_main_lora(String msg, Config &config);
};
