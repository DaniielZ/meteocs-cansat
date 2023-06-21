#pragma once
#include <Arduino.h>
#include <SPI.h>
class Config
{
public:
    struct Detection_parameter
    {
        float THRESHOLD;
        unsigned long int TIMESPAN;
    };
    struct Lora_device
    {
        long FREQUENCY;
        int CS;
        int RX;
        int TX;
        int SCK;
        int DIO0; // busy pin
        int DIO1;
        int RESET;
        int SYNC_WORD;

        int TXPOWER;
        int SPREADING;
        int CODING_RATE;
        long SIGNAL_BW;
        SPIClassRP2040 *SPI;
    };

    // logging
    unsigned long PC_BAUDRATE = 115200;
    bool WAIT_PC = false;
    bool LOG_TO_STORAGE = true;

    // GPS UART0
    int GPS_RX = 0;
    int GPS_TX = 1;
    long GPS_BAUDRATE = 9600;

    // LORA 433 SPI0
    Lora_device LORA433{
        .FREQUENCY = (long)430.6E6,
        .CS = 5,
        .RX = 4,
        .TX = 3,
        .SCK = 2,
        .DIO0 = 7,
        .DIO1 = 8,
        .RESET = 6,
        .SYNC_WORD = 0xF3,
        .TXPOWER = 14,
        .SPREADING = 9,
        .CODING_RATE = 6,
        .SIGNAL_BW = (long)62.5E3,
        .SPI = &SPI};

    // LORA 2.4 SPI1
    Lora_device LORA2400{
        .FREQUENCY = (long)2405.6E6,
        .CS = 13,
        .RX = 12,
        .TX = 11,
        .SCK = 10,
        .DIO0 = 16, // busy pin not programmable dont use
        .DIO1 = 15, // only use thsi
        .RESET = 14,
        .SYNC_WORD = 0xF4,
        .TXPOWER = 14,
        .SPREADING = 9,
        .CODING_RATE = 7,
        .SIGNAL_BW = (long)1600E3,
        .SPI = &SPI1};
    long RANGING_SLAVE_ADDRESS[3] = {0x12345671, 0x1234562, 0x12345673};
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
    int SERVO_PWM = 29;
    int SERVO_START = 1950;
    int SERVO_END = 1790;

    // Parachute
    int MOSFET = 22; // TBD

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1026.0; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    const int MAX_LOOP_TIME = 100; // ms
    // detection parameters
    const int DATA_POINTS_FOR_LAUNCH_DETECTION = 10;
    float LAUNCH_DETECTION_HEIGHT = 100; // delta m

    int TIME_FROM_LAUNCH_TO_DETECT_EJECTION = 20000;      // ms
    int TIME_AFTER_SOLAR_SAIL_TO_DEATACH_NANOSAT = 10000; // ms
    int TIME_AFTER_SOLAR_SAIL_TO_LAND = 200000;           // ms
    // ARMING AND DATA SENDING MSG IN PREP SATE

    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String SERVO_MSG = "servo_toggle";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};