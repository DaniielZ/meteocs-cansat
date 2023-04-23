#include <Arduino.h>
#include <SPI.h>
// gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <Wire.h>
// The TinyGPS++ object
TinyGPSPlus gps;

uint8_t GPS_RX = 0;
uint8_t GPS_TX = 1;
static const uint32_t GPS_BAUD = 9600;

SoftwareSerial ss(GPS_RX, GPS_TX);

void setup()
{
    Serial.begin(115200);
    delay(500);
    while (!Serial)
    {
        delay(1000);
    }
    ss.begin(GPS_BAUD);
    Serial.println("GPS initialized!");
}

void loop()
{
    while (ss.available() > 0)
    {
        gps.encode(ss.read());
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
    }
}