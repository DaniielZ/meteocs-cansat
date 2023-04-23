#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// TESTED
uint8_t LIS2MDL_SCL = 21;
uint8_t LIS2MDL_SDA = 20;
/* Assign a unique ID to this sensor at the same time */
Adafruit_LIS2MDL lis2mdl = Adafruit_LIS2MDL(12345);

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("LIS2MDL Magnetometer Test");
    Serial.println("");

    /* Enable auto-gain */
    lis2mdl.enableAutoRange(true);
    Wire.setSCL(21);
    Wire.setSDA(20);
    /* Initialise the sensor */
    if (!lis2mdl.begin())
    { // I2C mode
        // if (! lis2mdl.begin_SPI(LIS2MDL_CS)) {  // hardware SPI mode
        // if (! lis2mdl.begin_SPI(LIS2MDL_CS, LIS2MDL_CLK, LIS2MDL_MISO, LIS2MDL_MOSI)) { // soft SPI
        /* There was a problem detecting the LIS2MDL ... check your connections */
        Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
        while (1)
            delay(10);
    }

    /* Display some basic information on this sensor */
    lis2mdl.printSensorDetails();
}

void loop(void)
{
    /* Get a new sensor event */
    sensors_event_t event;
    lis2mdl.getEvent(&event);

    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: ");
    Serial.print(event.magnetic.x);
    Serial.print("  ");
    Serial.print("Y: ");
    Serial.print(event.magnetic.y);
    Serial.print("  ");
    Serial.print("Z: ");
    Serial.print(event.magnetic.z);
    Serial.print("  ");
    Serial.println("uT");

    /* Delay before the next sample */
    delay(100);
}