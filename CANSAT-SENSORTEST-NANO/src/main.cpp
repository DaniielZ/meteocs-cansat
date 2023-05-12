#include <Arduino.h>
#include <SPI.h>
// gps
#include <TinyGPS++.h>
#include "SoftwareSerial.h"
#include <string.h>
#include <Wire.h>
// The TinyGPS++ object
TinyGPSPlus gps;

uint8_t GPS_RX = 0;
uint8_t GPS_TX = 1;
static const uint32_t GPS_BAUD = 9600;
#define ss Serial1
char cat;
uint8_t serial_buffer[255];
void setup()
{
    Serial.begin(115200);
    delay(500);
    while (!Serial)
    {
        delay(1000);
    }
    ss.begin(GPS_BAUD);
    ss.addMemoryForRead(serial_buffer, sizeof(serial_buffer));
    Serial.println("GPS initialized!");
}

void loop()
{
    while (ss.available())
    {
        cat = (ss.read());
        gps.encode(cat);
        Serial.print(cat);
        if (cat == '%')
            Serial.println(" ");
    }

    Serial.println("");
    Serial.println("");
    if (gps.location.isUpdated())
    {
        // Number of satellites in use (u32)
        Serial.print("Number os satellites in use = ");
        Serial.println(gps.satellites.value());
        // Latitude in degrees (double)
        Serial.print("Latitude= ");
        Serial.print(gps.location.lat(), 6);
        // Longitude in degrees (double)
        Serial.print(" Longitude= ");
        Serial.println(gps.location.lng(), 6);
    }
    delay(500);
}