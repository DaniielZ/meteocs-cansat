// rx 4, tx 3 sck 2 cs 17 spi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {

        ; // wait for serial port to connect. Needed for native USB port only
    }
    pinMode(20, OUTPUT_12MA);
    Serial.println("Initializing SD card...\n");
    SPI.setRX(4);
    // SPI.setCS(17);
    SPI.setSCK(2);
    SPI.setTX(3);
    SPI.begin();
    if (!SD.begin(20, SPI))
    {
        Serial.println("SD Card initialization failed.");
        return;
        // while(1);
    }
    else
    {
        Serial.println("initialization done.");
    } // end if

    File f = SD.open("user_scales.ini", "w+");
    if (!f)
    {
        Serial.printf("file open failed.\n");
        // return ini;
    } // end if

    f.close();
}

void loop()
{
}