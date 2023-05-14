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
    bool LOG_TO_STORAGE = true;

    // USING INTERNAL MEMORY INSTED !!!!!
    // // flash SPI1
    // int FLASH_CS = 13;
    // int FLASH_RX = 12;
    // int FLASH_TX = 11;
    // int FLASH_SCK = 10;
    // int FLASH_WP = 9;
    // int FLASH_HOLD = 8;
    // int FLASH_JDEC = 0x1F85;

    // GPS UART0
    int GPS_RX = 0;
    int GPS_TX = 1;
    long GPS_BAUDRATE = 9600;

    // MAGNETO WIRE0
    int LIS2MDL_SCL = 21;
    int LIS2MDL_SDA = 20;

    // LORA SPI0
    long LORA_FREQUENCY = 430.6E6; // 434.25E6
    int LORA_CS = 5;
    int LORA_RX = 4;
    int LORA_TX = 3;
    int LORA_SCK = 2;
    int LORA_TXPOWER = 99; // max :)
    int LORA_SPREADING = 10;
    int LORA_CODING_RATE = 7;
    long LORA_SIGNAL_BW = 62.5E3;

    // BARO WIRE1 || CONFLICT WITH SHTC3
    int MS5611_SCL = 27;
    int MS5611_SDA = 26;
    int MS5611_ADDRESS = 0x77;

    // HUMIDITY WIRE1 || CONFLICT WITH MS611 bet koda fixos
    int SHTC3_SCL = 19;
    int SHTC3_SDA = 18;

    // BUZZER
    int BUZZER = 6;
    int BUZZER_LONG_BEEP = 300;
    int BUZZER_LONG_PAUSE = 300;
    int BUZZER_SHORT_BEEP = 50;
    int BUZZER_SHORT_PAUSE = 50;
    int BUZZER_ERROR_BEEPS = 20;

    // Ejection
    // int SERVO_PWM = 22;
    // int SERVO_START_POS = 0;
    // int SERVO_END_POS = 180;
    // Parachute
    int MOSFET = 22; // TBD

    // Photo resistor
    int PHOTO_ADC = 28;

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1026.0; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    int SLEEP = 200; // ms

    Detection_parameter HARD_LOCK_HEIGHT = {570, 5000}; // makse sure to change the max array size if needed
    // Detection_parameter PARACHUTE_LIGHT = {800, 2000};
    Detection_parameter LANDED_HEIGHT = {200, 10000};
    // ARMING AND DATA SENDING MSG IN PREP SATE
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};