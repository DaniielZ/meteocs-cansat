#include <Arduino.h>
#include <Wire.h>
#include "H3LIS100.h"
int sda = 16;
int scl = 17;

TwoWire acc_wire(i2c0, sda, scl);

void setup()
{
    Serial.begin();
    while (!Serial)
    {
        delay(100);
    }
    delay(500);
    H3LIS100 acc(123);
    if (acc.begin(0x19, &acc_wire))
    {
        Serial.println("alles good");
    }
    Serial.println("alles good");
    while (true)
    {
        sensors_event_t event;
        acc.getEvent(&event);
        Serial.print(event.acceleration.x);
        Serial.print(event.acceleration.y);
        Serial.println(event.acceleration.z);
        delay(500);
    }
}

void loop()
{
}