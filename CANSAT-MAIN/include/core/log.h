#pragma once
#include "core/cansat.h"

class Cansat;
class Log
{
    SPIClassRP2040 SPI_LORA;
    LoRaClass LoRaMain;
    SPIClassRP2040 SPI_FLASH;
    SPIFlash flash;

public:
    void init(Cansat &cansat);
    void info(char msg[]);
    void error(Cansat &cansat, char msg[]);
    void data(Cansat &cansat);
    bool read(Cansat &cansat, String &msg);
};
