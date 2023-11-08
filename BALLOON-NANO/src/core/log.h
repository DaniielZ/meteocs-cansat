#pragma once
#include <SPI.h>
#include <RadioLib.h>
#include <Arduino.h>
#include "config.h"
#include "sensors/sensor_manager.h"

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
    void update_data_packet(Sensor_manager::Sensor_data &data, String &result_sent, String &result_log);
    
    void transmit_data(Config &config);
    void log_data_to_flash();
    void log_data_to_pc();
    
    void read(String &msg);
    bool format_storage(Config &config);
    bool send_main_lora(String msg, Config &config);
};
