#include <Arduino.h>
#include "Wire.h"
//WORKING


//
//    FILE: MS5611_minimal.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo application
//    DATE: 2021-12-24
//     URL: https://github.com/RobTillaart/MS5611

#include "MS5611.h"

//  CS to VCC  ==>  0x76
//  CS to GND  ==>  0x77

MS5611 ms5611(0x77);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println();
    Serial.println(__FILE__);
    Serial.print("MS5611_LIB_VERSION: ");
    Serial.println(MS5611_LIB_VERSION);
    Wire1.setSCL(27);
    Wire1.setSDA(26);

    if (ms5611.begin(&Wire1) == true)
    {
        Serial.println("MS5611 found.");
    }
    else
    {
        Serial.println("MS5611 not found. halt.");
        while (1)
            ;
    }
    Serial.println();
}

void loop()
{
    ms5611.read(); // note no error checking => "optimistic".
    Serial.print("T:\t");
    Serial.print(ms5611.getTemperature(), 2);
    Serial.print("\tP:\t");
    Serial.print(ms5611.getPressure(), 2);
    Serial.println();
    delay(1000);
}

// -- END OF FILE --