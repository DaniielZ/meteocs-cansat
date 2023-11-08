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
    String _log_file_path_final;
    bool _flash_initialized = false;
    void init_flash(Config &config);
    void data_to_packet(Sensor_manager::Sensor_data &data, String &result_log, String &result_sent);

public:
    void init(Config &config);
    void info(String msg, Config &config);
    void data(Sensor_manager::Sensor_data &data, bool log_to_storage = true, bool transmit = true, Config &config);
    void read(String &msg);
    bool format_storage(Config &config);
    bool send_main_lora(String msg, Config &config);
};
