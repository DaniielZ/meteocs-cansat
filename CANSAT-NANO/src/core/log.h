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

    String _log_file_path_final;
    bool _flash_initialized = false;
    void init_flash(Config &config);
    void init_lora(Config::Lora_device &lora_cfg);
    void init_pcserial(Config &config);

public:
    void init(Config &config);
    void info(String msg);
    void data(Sensor_manager::Sensor_data &data, bool log_to_storage = true);
    void read(String &msg);
};
