// flash dumper
#include <LittleFS.h>
#include <Arduino.h>
// .ini settings
// [env:log_output_to_file]
// platform = … monitor_filters = default, log2file

String LOG_FILE_NAME_BASE_PATH = "/CANSAT";

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Serial.print("Inizializing FS...");
    if (LittleFS.begin())
    {
        Serial.println("Done.");
    }
    else
    {
        Serial.println("Fail.");
    }
}
void dump_data()
{
    Serial.print("Dumping...");
    // determine nr for final path
    int log_file_name_nr = 0;
    while (LittleFS.exists(LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt"))
    {
        log_file_name_nr++;
    }
    log_file_name_nr--;
    String log_file_path_final = LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt";
    File file = LittleFS.open(log_file_path_final, "r");
    if (!file)
    {
        Serial.println("Failed reading : " + log_file_path_final);
    }
    else
    {
        Serial.println("Opened file : " + log_file_path_final);
    }

    while (file.available() > 0)
    {
        String data = file.readString();
        Serial.println(data);
    }
    file.close();
    Serial.println("Done dumping, do you want to delete the file? Yes/No");
    while (true)
    {

        while (Serial.available() == 0)
        {
        };
        String incoming_msg = Serial.readString();
        if (incoming_msg == "Yes")
        {
            if (LittleFS.remove(log_file_path_final))
            {
                Serial.println("Removed : " + log_file_path_final);
            }
            else
            {
                Serial.println("Failed removing : " + log_file_path_final);
            }
        }
        if (incoming_msg == "No")
        {
            break;
        }
    }
}
void loop()
{
    Serial.println("Type Erase to format, Type Dump to dump latest file");
    while (Serial.available() == 0)
    {
    }
    String incoming_msg = Serial.readString();
    if (incoming_msg == "Erase")
    {
        Serial.print("Earsing...");
        if (LittleFS.format())
        {
            Serial.println("Done");
        }
        else
        {
            Serial.println("Fail");
        }
    }
    else if (incoming_msg == "Dump")
    {
        dump_data();
    }
    else
    {
        Serial.println("Unexpected input : " + incoming_msg);
    }
}
