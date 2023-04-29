#pragma once
#include "core/cansat.h"

class Cansat;
class Log
{
    SPIClassRP2040 _spi_lora;
    LoRaClass _lora;
    SPIClassRP2040 _spi_flash;
    SPIFlash _flash;
    void init_flash(Cansat &cansat);
    void init_lora(Cansat &cansat);

public:
    void init(Cansat &cansat);
    void info(char msg[]);
    void error(Cansat &cansat, char msg[]);
    void data(Cansat &cansat);
    bool read(Cansat &cansat, String &msg);
};
