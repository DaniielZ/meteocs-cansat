#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1); // 0x29
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Wire1.setSCL(27);
    Wire1.setSDA(26);

    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }
    while (true)
    {
        sensors_event_t angVelocityData, accelerometerData;
        bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        Serial.print(angVelocityData.gyro.x);
        Serial.print(", ");
        Serial.print(angVelocityData.gyro.y);
        Serial.print(", ");
        Serial.print(angVelocityData.gyro.z);
        Serial.println(": Gyro");
        Serial.print(accelerometerData.acceleration.x);
        Serial.print(", ");
        Serial.print(accelerometerData.acceleration.y);
        Serial.print(", ");
        Serial.print(accelerometerData.acceleration.z);
        Serial.println(": acc");
    }
}
void loop()
{
}
