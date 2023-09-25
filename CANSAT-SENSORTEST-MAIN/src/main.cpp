#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
int mosfet = 9;
int buzzer = -1;
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }
    pinMode(mosfet, OUTPUT_12MA);
    pinMode(buzzer, OUTPUT_12MA);
}
void loop()
{
    Serial.println("ON");
    digitalWrite(mosfet, HIGH);
    for (int i = 0; i < 50; i++)
    {
        digitalWrite(buzzer, HIGH);
        delay(50);
        digitalWrite(buzzer, HIGH);
        delay(50);
    }
    Serial.println("OFF");
    digitalWrite(mosfet, LOW);
    digitalWrite(buzzer, LOW);
    delay(5000);
}