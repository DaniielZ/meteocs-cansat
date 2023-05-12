#include <Arduino.h>
#include <SPI.h>
// gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <Wire.h>
// The TinyGPS++ object
TinyGPSPlus gps;

uint8_t GPS_RX = 1;
uint8_t GPS_TX = 0;
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
    Serial1.setFIFOSize(128);
    Serial1.begin(GPS_BAUD);
    Serial.println("GPS initialized!");
}

void loop()
{
    while (Serial1.available() > 0)
    {
        char result = Serial1.read();
        gps.encode(result);
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