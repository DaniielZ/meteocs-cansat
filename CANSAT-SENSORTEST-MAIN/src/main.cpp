#include <Arduino.h>
#include <SPI.h>
// gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <Wire.h>
#include <Vector.h>
// The TinyGPS++ object
TinyGPSPlus gps;

uint8_t GPS_RX = 1;
uint8_t GPS_TX = 0;
static const uint32_t GPS_BAUD = 9600;
// Photo resistor
int PHOTO_ADC = 28;

SoftwareSerial ss(GPS_RX, GPS_TX);

void setup()
{
    Serial.begin(115200);
    delay(500);
    while (!Serial)
    {
        delay(1000);
    }
}

void loop()
{
    Vector<int> list();
    list.push_back(1);
    list.push_back(2);
    Serial.println(list.size());
    delay(2000);
}