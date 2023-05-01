#include <Arduino.h>
#include <SPI.h>

class Test
{
    SPIClassRP2040 _spi();
    int x;
};

void setup()
{
    Test test;
}
void loop()
{
}