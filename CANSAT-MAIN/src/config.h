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

    // GPS UART0
    int GPS_RX = 0;
    int GPS_TX = 1;
    long GPS_BAUDRATE = 9600;

    // LORA SPI0
    long LORA_FREQUENCY = 430.6E6;
    int LORA_CS = 5;
    int LORA_RX = 4;
    int LORA_TX = 3;
    int LORA_SCK = 2;
    int LORA_TXPOWER = 99; // max :)
    int LORA_SPREADING = 10;
    int LORA_CODING_RATE = 7;
    long LORA_SIGNAL_BW = 62.5E3;

    // WIRE1 lines
    int WIRE1_SCL = 27;
    int WIRE1_SDA = 26;

    // BARO WIRE1
    int MS5611_ADDRESS = 0x77;

    // HUMIDITY WIRE1
    int SHTC3_ADDRESS = 0x70;

    // BNO055 WIRE1
    int BNO055_ADDRESS = 0x28; // or 29

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
    int MOSFET = 22; // TBD

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1026.0; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    int MAX_DATA_RATE = 10; // Hz

    // Detection_parameter HARD_LOCK_HEIGHT = {570, 5000}; // makse sure to change the max array size if needed
    // // Detection_parameter PARACHUTE_LIGHT = {800, 2000};
    // Detection_parameter LANDED_HEIGHT = {200, 10000};
    // // ARMING AND DATA SENDING MSG IN PREP SATE

    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};