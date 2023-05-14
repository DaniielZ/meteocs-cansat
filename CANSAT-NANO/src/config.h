#pragma once
#include <Arduino.h>
class Config
{
public:
    struct Detection_parameter
    {
        float THRESHOLD;
        unsigned long int TIMESPAN;
    };
    // logging
    unsigned long PC_BAUDRATE = 115200;
    bool WAIT_PC = false;
    bool LOG_TO_STORAGE = false;

    // USING INTERNAL MEMORY INSTED !!!!!
    // // flash SPI1
    // int FLASH_CS = 13;
    // int FLASH_RX = 12;
    // int FLASH_TX = 11;
    // int FLASH_SCK = 10;
    // int FLASH_WP = 9;
    // int FLASH_HOLD = 8;
    // int FLASH_JDEC = 0x1F85;

    // GPS UART1
    int GPS_RX = 0;
    int GPS_TX = 1;
    long GPS_BAUDRATE = 9600;

    // MAGNETO WIRE0
    // int LIS2MDL_SCL = 21;
    // int LIS2MDL_SDA = 20;

    // LORA SPI0
    long LORA_FREQUENCY = 430.575E6;
    int LORA_CS = 6;
    int LORA_RX = 8;
    int LORA_TX = 7;
    int LORA_SCK = 27;
    int LORA_TXPOWER = 99; // max :)
    int LORA_SPREADING = 10;
    int LORA_CODING_RATE = 7;
    long LORA_SIGNAL_BW = 62.5E3;
    // ACC WIRE2
    int ACC_SDA = 4;
    int ACC_SCL = 3;
    // BARO WIRE0
    int MS5611_SCL = 19;
    int MS5611_SDA = 18;
    int MS5611_ADDRESS = 0x77;

    // HUMIDITY WIRE1
    int SHTC3_SCL = 37;
    int SHTC3_SDA = 38;

    // BUZZER
    int BUZZER = -1;
    int BUZZER_LONG_BEEP = 300;
    int BUZZER_LONG_PAUSE = 300;
    int BUZZER_SHORT_BEEP = 50;
    int BUZZER_SHORT_PAUSE = 50;
    int BUZZER_ERROR_BEEPS = 20;

    // Ejection
    int SERVO_PWM = 29;
    int SERVO_START = 1950; // 1950
    int SERVO_END = 1750;   // 1840
    // Parachute
    // int MOSFET = 22; // TBD

    // Photo resistor
    // int PHOTO_ADC = 28;

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1026; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    int SLEEP = 100; // ms

    // !!!!!!!!!! THE ALTITUDE IS IN SEA LEVEL DONT FUCK THIS UP!!!!!!!!!!!!!!!!!!!!
    Detection_parameter HARD_LOCK_HEIGHT = {950, 5000}; // makse sure to change the max array size if needed
    Detection_parameter EJECTION_HEIGHT = {900, 3000};
    Detection_parameter LANDED_HEIGHT = {200, 10000};
    // ARMING AND DATA SENDING MSG IN PREP SATE
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String SERVO_MSG = "servo_toggle";

    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};