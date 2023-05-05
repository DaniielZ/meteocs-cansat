#include <Arduino.h>
#include <Wire.h>
#include "H3LIS100.h"
int sda = 16;
int scl = 17;

TwoWire acc_wire(i2c0, sda, scl);

void scan_address()
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        acc_wire.beginTransmission(address);
        error = acc_wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}
void setup()
{
    Serial.begin();
    while (!Serial)
    {
        delay(100);
    }
    delay(500);
    scan_address();
    H3LIS100 acc(123);
    if (acc.begin(24u, &acc_wire))
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