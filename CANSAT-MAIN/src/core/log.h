#pragma once
#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include "config.h"
#include "sensors/sensor_manager.h"
class Log
{
    SPIClassRP2040 *_spi_lora;
    LoRaClass _lora;
    int _log_file_name_nr = 0; // set by init function
    void init_flash(Config &config);
    void init_lora(Config &config);
    String _log_file_path_final;

public:
    void init(Config &config);
    void info(String msg);
    void data(Sensor_manager::Sensor_data &data, bool log_to_storage = true);
    bool read(String &msg);
};
