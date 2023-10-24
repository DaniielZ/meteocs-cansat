#pragma once
#include <SPI.h>
#include <RadioLib.h>
#include <Arduino.h>
#include "config.h"
#include "sensors/sensor_manager.h"
class Log
{
    RFM96 _lora = new Module(5, 7, 6, 8, SPI);
    bool _lora_initialized = false;
    FS *_flash;
    String _log_file_path_final;
    bool _flash_initialized = false;
    void init_flash(Config &config);
    void init_lora(Ranging_Wrapper::Lora_Device &lora_cfg);
    void data_to_packet(Sensor_manager::Sensor_data &data, String &result_log, String &result_sent);

public:
    void init(Config &config);
    void info(String msg);
    void data(Sensor_manager::Sensor_data &data, bool log_to_storage = true, bool transmit = true);
    void read(String &msg);
    bool format_storage(Config &config);
};
