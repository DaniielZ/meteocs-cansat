#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
int mosfet = 22;
int buzzer = 6;
void setup()
{
    Serial.begin(9600);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    pinMode(mosfet, OUTPUT_12MA);
    pinMode(buzzer, OUTPUT_12MA);
}
void loop()
{
    delay(10000);
    Serial.println("ON");
    digitalWrite(mosfet, HIGH);
    for (int i = 0; i < 30; i++)
    {
        digitalWrite(buzzer, HIGH);
        delay(50);
        digitalWrite(buzzer, LOW);
        delay(50);
    }
    Serial.println("OFF");
    digitalWrite(mosfet, LOW);
    digitalWrite(buzzer, LOW);
}