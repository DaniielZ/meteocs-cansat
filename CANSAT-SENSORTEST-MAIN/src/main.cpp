#include <Arduino.h>
#include <SPI.h>
// gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>

// The TinyGPS++ object
TinyGPSPlus gps;

uint8_t GPS_RX = 1;
uint8_t GPS_TX = 0;
static const uint32_t GPS_BAUD = 9600;

SerialPIO ss(GPS_TX, GPS_RX, 500);

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
        char ss_result = ss.read();
        Serial.print((int)ss_result);
        gps.encode((char)ss_result);
        Serial.print(":");
        Serial.print((char)ss_result);
        Serial.print("   ");
    }
    delay(100);
    Serial.println(" ");
}