#pragma once
#include <SPI.h>
#include <LoRa.h>
#include <LittleFS.h>
#include <Sensor.

class Cansat;
class Log
{
    SPIClassRP2040 _spi_lora();
    LoRaClass _lora;
    int _log_file_name_nr = 0; // set by init function
    void init_flash(Cansat &cansat);
    void init_lora(Cansat &cansat);
    String _log_file_path_final;

public:
    void init(Cansat &cansat);
    void info(char msg[]);
    void error(Cansat &cansat, char msg[]);
    void data(Cansat &cansat);
    bool read(Cansat &cansat, String &msg);
};
