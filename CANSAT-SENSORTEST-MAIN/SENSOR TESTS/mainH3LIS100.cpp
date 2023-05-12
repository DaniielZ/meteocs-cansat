#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "H3LIS100.h"
int sda = 16;
int scl = 17;

void setup()
{
    Wire1.setSCL(15);
    Wire1.setSDA(14);
    Wire1.begin();
    Serial.begin();
    while (!Serial)
    {
        delay(100);
    }
    delay(500);
    H3LIS100 acc(123);
    if (!acc.begin(0x19, &Wire1))
    {
        Serial.println("alles bad");
        while (true)
        {
            delay(100);
        }
    }
    while (true)
    {
        sensors_event_t event;
        acc.getEvent(&event);
        Serial.print(event.acceleration.x);
        Serial.print(", ");
        Serial.print(event.acceleration.y);
        Serial.print(", ");
        Serial.println(event.acceleration.z);
        delay(500);
    }
}

void loop()
{
}