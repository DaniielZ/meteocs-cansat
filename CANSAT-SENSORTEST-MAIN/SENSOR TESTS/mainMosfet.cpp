#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
int mosfet = 22;
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }
    pinMode(mosfet, OUTPUT_12MA);
}
void loop()
{
    Serial.println("ON");
    digitalWrite(mosfet, HIGH);
    delay(1000);
    Serial.println("OFF");
    digitalWrite(mosfet, LOW);
    delay(1000);
}