#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>
int SERVO_PWM = 29;
int SERVO_START_POS = 0;
int SERVO_END_POS = 180;
Servo servo;
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }
    delay(1000);
    Serial.println("HI");

    servo.attach(SERVO_PWM);
    delay(100);
}
void loop()
{
    servo.writeMicroseconds(1700);
    Serial.println("START");
    delay(1000);
    servo.writeMicroseconds(1900);
    Serial.println("END");
    delay(1000);
}