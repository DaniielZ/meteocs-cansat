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
        float FREQUENCY;
        int CS;
        int DIO0; // busy pin
        int DIO1;
        int RESET;
        int SYNC_WORD;

        int TXPOWER;
        int SPREADING;
        int CODING_RATE;
        float SIGNAL_BW;
        SPIClassRP2040 *SPI;
    };

    // logging
    unsigned long PC_BAUDRATE = 115200;
    bool WAIT_PC = true;
    bool LOG_TO_STORAGE = true;
    //
    bool START_RANGING_FIRST = false;

    // GPS UART0
    int SERIAL1_RX = 17;
    int SERIAL1_TX = 16;
    long SERIAL1_BAUDRATE = 9600;

    // Wire0
    int WIRE0_SCL = 1;
    int WIRE0_SDA = 0;
    // Wire1
    // int WIRE1_SCL = -1;
    // int WIRE1_SDA = -1;

    // SPI0
    int SPI0_RX = 4;
    int SPI0_TX = 3;
    int SPI0_SCK = 2;
    // SPI1
    int SPI1_RX = 12;
    int SPI1_TX = 11;
    int SPI1_SCK = 10;

    // LORA 433 SPI0
    Lora_device LORA433{
        .FREQUENCY = 433.575,
        .CS = 5,
        .DIO0 = 7,
        .DIO1 = 8,
        .RESET = 6,
        .SYNC_WORD = 0xF4,
        .TXPOWER = 14,
        .SPREADING = 9,
        .CODING_RATE = 6,
        .SIGNAL_BW = 125,
        .SPI = &SPI};

    // LORA 2.4 SPI1
    Lora_device LORA2400{
        .FREQUENCY = 2405.6,
        .CS = 13,
        .DIO0 = 18, // busy pin not programmable dont use
        .DIO1 = 15, // only use thsi
        .RESET = 10,
        .SYNC_WORD = 0xF5,
        .TXPOWER = 14,
        .SPREADING = 9,
        .CODING_RATE = 7,
        .SIGNAL_BW = 1600,
        .SPI = &SPI1};
    struct Ranging_slave
    {
        float lat;
        float lng;
        float height;
        long address;
    };

    Ranging_slave RANGING_SLAVES[3] = {{.lat = 0, .lng = 0, .height = 0, .address = 0x12345671},
                                       {.lat = 0, .lng = 0, .height = 0, .address = 0x12345672},
                                       {.lat = 0, .lng = 0, .height = 0, .address = 0x12345673}};

    int RANGING_TIMEOUT = 100; // ms
    int SD_CARD_CS = -1;
    SPIClassRP2040 *SD_CARD_SPI = &SPI1;

    // BARO WIRE0
    int MS5611_ADDRESS = 0x77;

    // HUMIDITY WIRE0
    int SHTC3_ADDRESS = 0x70;

    // BNO055 WIRE0
    int BNO055_ADDRESS = 0x29; // or 29

    // INNER TEMP WIRE0
    int STS35_ADDRESS = 0x48;

    // OUTTTER TEMP ANALOG
    int THERMISTOR_PIN = -1;

    // BUZZER
    int BUZZER = -1;
    int BUZZER_LONG_BEEP = 300;
    int BUZZER_LONG_PAUSE = 300;
    int BUZZER_SHORT_BEEP = 50;
    int BUZZER_SHORT_PAUSE = 50;
    int BUZZER_ERROR_BEEPS = 20;

    // Parachute
    int MOSFET = -1; // TBD

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1015.0; // CHNAGE BEFORE FLIGHT;

    // hard data rate limiter
    const int MAX_LOOP_TIME = 5; // ms
    // detection parameters
    const int DATA_POINTS_FOR_LAUNCH_DETECTION = 5;
    float LAUNCH_DETECTION_HEIGHT = 100; // delta m

    int TIME_FROM_LAUNCH_TO_DETECT_EJECTION = 20000;      // ms
    int TIME_AFTER_SOLAR_SAIL_TO_DEATACH_NANOSAT = 10000; // ms
    int TIME_AFTER_SOLAR_SAIL_TO_LAND = 200000;           // ms
    // ARMING AND DATA SENDING MSG IN PREP SATE
    String RANGE_DONE = "range_done"; // used by both sats to tell which one should be transmititing
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String SERVO_MSG = "servo_toggle";
    String FORMAT_MSG = "format_flash";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};