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
    Serial.println("GPS initialized1!");
}

void loop()
{
    while (ss.available() > 0)
    {
        char ss_result = ss.read();
        gps.encode((char)ss_result);
        Serial.print(ss_result);
        if (gps.location.isValid())
        {
            Serial.println(gps.location.lat(), 6);
            Serial.println(gps.location.lat(), 6);
            Serial.println(gps.time.value());
        }
    }
    delay(100);
    Serial.println(" ");
}