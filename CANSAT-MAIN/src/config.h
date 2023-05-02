#pragma once
#include <Arduino.h>
class Config
{
public:
    // logging
    unsigned long PC_BAUDRATE = 115200;
    bool WAIT_PC = true;

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
    long LORA_FREQUENCY = 434.1E6;
    int LORA_CS = 5;
    int LORA_RX = 4;
    int LORA_TX = 3;
    int LORA_SCK = 2;
    int LORA_TXPOWER = 10;
    int LORA_SPREADING = 10;
    int LORA_CODING_RATE = 6;
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
    int SERVO_PWM = 22;
    int SERVO_START_POS = 0;
    int SERVO_END_POS = 180;
    // Parachute
    int MOSFET = 0; // TBD

    // Photo resistor
    int PHOTO_ADC = 28;
    // Cansat is landed parameters
    float ALTITUDE_STD = 5;       // m
    int ALTITUDE_STD_TIME = 3000; // ms

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1013.25; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    int SLEEP = 100; // ms

    // detection parameters
    unsigned long FALLING_TIME_SPAN = 1000;
    float FALING_TRESHOLD_ACC_MIN = 0;
    float FALING_TRESHOLD_ACC_MAX = 2;

    unsigned long LANDING_TIME_SPAN = 10000;
    float LANDING_TRESHOLD_ACC_MIN = 8;
    float LANDING_TRESHOLD_ACC_MAX = 11;
    // ARMING AND DATA SENDING MSG IN PREP SATE
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";

    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};