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
    bool _lora_initialized = false;

    String _log_file_path_final;
    bool _flash_initialized = false;

    void init_flash(Config &config);
    void init_lora(Config &config);
    void init_pcserial(Config &config);

public:
    void init(Config &config);
    void info(String msg);
    void data(Sensor_manager::Sensor_data &data, bool log_to_storage = true);
    void read(String &msg);
};
