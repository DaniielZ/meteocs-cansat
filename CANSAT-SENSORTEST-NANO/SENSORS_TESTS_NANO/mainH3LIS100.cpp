#include <Arduino.h>
#include <Wire.h>
#include "H3LIS100.h"
int sda = 4;
int scl = 3;

// TwoWire acc_wire(i2c0, sda, scl);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }
    delay(500);
    H3LIS100 acc(123);
    Wire2.setSCL(scl);
    Wire2.setSDA(sda);
    if (!acc.begin(0x19, &Wire2))
    {
        Serial.println("alles bad");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("alles good");
    delay(1000);
    while (true)
    {
        sensors_event_t event;
        acc.getEvent(&event);
        Serial.print(event.acceleration.x, 4);
        Serial.print(" ");
        Serial.print(event.acceleration.y, 4);
        Serial.print(" ");
        Serial.println(event.acceleration.z, 4);
        delay(100);
    }
}
void loop()
{
}