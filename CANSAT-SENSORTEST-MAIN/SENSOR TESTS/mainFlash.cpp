/*
 *  Raspberry Pi Pico (or generic rp2040)
 *  LittleFS get info, read dir and show all file uploaded
 *  add a data folder to use with Pico LittleFS Data Upload
 *  by Mischianti Renzo <https://www.mischianti.org>
 *
 *  https://www.mischianti.org
 *
 */

#include "Arduino.h"
#include "LittleFS.h"

void printDirectory(File dir, int numTabs = 6);

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(100);
    }

    Serial.println("Inizializing FS...");
    if (LittleFS.begin())
    {
        Serial.println("done.");
    }
    else
    {
        Serial.println("fail.");
    }

    // To format all space in LittleFS
    // LittleFS.format()

    // Get all information of your LittleFS
    FSInfo *info;
    LittleFS.info(*info);

    unsigned int totalBytes = info->totalBytes;
    unsigned int usedBytes = info->usedBytes;
    unsigned int freeBytes = totalBytes - usedBytes;

    unsigned int maxPath = info->maxPathLength;

    Serial.println("File sistem info.");

    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");

    Serial.print("Total space free: ");
    Serial.print(freeBytes);
    Serial.println("byte");

    Serial.print("Max path lenght: ");
    Serial.print(maxPath);
    Serial.println("");

    Serial.println();

    // Open dir folder
    File dir = LittleFS.open("/", "r");
    // Cycle all the content
    printDirectory(dir);
}

void loop()
{
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.print(entry.size(), DEC);
            Serial.print("\t");
            Serial.println("byte");
        }
        entry.close();
    }
}