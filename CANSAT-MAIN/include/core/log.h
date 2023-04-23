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
    void info();
    void error();
    void data();
};
