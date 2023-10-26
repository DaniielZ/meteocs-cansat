#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <LittleFS.h>
#include <SDFS.h>
#include <ranging_wrapper.h>
class Config
{
public:
    //------------ OFTEN CHANGED ------------------
    // hard data rate limiter
    const int MAX_LOOP_TIME = 20; // ms

    bool WAIT_PC = false;
    bool LOG_TO_STORAGE = true;

    Ranging_Wrapper::Ranging_Slave RANGING_SLAVES[3] = {{.position = {0, 0, 0}, .address = 0x12345678},
                                                        {.position = {0, 0, 0}, .address = 0xABCD9876},
                                                        {.position = {0, 0, 0}, .address = 0x9A8B7C6D}};

    // LORA 433 SPI0
    Ranging_Wrapper::Lora_Device LORA433{
        .FREQUENCY = 434.5,
        .CS = 5,
        .DIO0 = 7,
        .DIO1 = 8,
        .RESET = 6,
        .SYNC_WORD = 0xF4,
        .TXPOWER = 14,
        .SPREADING = 10,
        .CODING_RATE = 7,
        .SIGNAL_BW = 125,
        .SPI = &SPI};

    // LORA 2.4 SPI1
    Ranging_Wrapper::Lora_Device LORA2400{
        .FREQUENCY = 2405.6,
        .CS = 13,
        .DIO0 = 18,  // busy pin not programmable dont use
        .DIO1 = 15,  // only use thsi
        .RESET = 14, // 10
        .SYNC_WORD = 0xF5,
        .TXPOWER = 14,
        .SPREADING = 10,
        .CODING_RATE = 7,
        .SIGNAL_BW = 406.25,
        .SPI = &SPI1};

    float HEATER_CUT_OFF_VOLTAGE = 6.0; // V
    float DESIRED_HEATER_TEMP = 35.0;   // in C
    unsigned int OUTER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int OUTER_TEMP_AVERAGE_CAPACITY = ((OUTER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    unsigned int INNER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int INNER_TEMP_AVERAGE_CAPACITY = ((INNER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    unsigned int BAT_AVERAGE_TIME = 5000;
    const unsigned int BAT_AVERAGE_CAPACITY = ((BAT_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);

    const int LORA_DATAPACKET_COOLDOWN = 5000; // in ms  30000
    int TIME_FROM_LAUNCH_TO_EJECT = 20000;     // ms
    int MOSFET_ON_TIME = 10000;                // ms

    //----------------------------------SOMEWHAT STATIC -------------------------------------

    // logging
    unsigned long PC_BAUDRATE = 115200;
    FS *FILE_SYSTEM = &SDFS; // if change to LittleFS need to change some code
    // GPS UART0
    int SERIAL1_RX = 17;
    int SERIAL1_TX = 16;
    long SERIAL1_BAUDRATE = 9600;
    // Wire0
    int WIRE0_SCL = 1;
    int WIRE0_SDA = 0;
    // SPI0
    int SPI0_RX = 4;
    int SPI0_TX = 3;
    int SPI0_SCK = 2;
    // SPI1
    int SPI1_RX = 12;
    int SPI1_TX = 11;
    int SPI1_SCK = 10;

    Ranging_Wrapper::Mode LORA2400_MODE = Ranging_Wrapper::Mode::MASTER;

    int RANGING_TIMEOUT = 500; // ms  might need to adjust this if lora parameters change
    int SD_CARD_CS = 20;

    SPIClassRP2040 *SD_CARD_SPI = &SPI;

    int BMP180_ADDRESS_I2C = 0x77; // or 0x76 :D

    // INNER TEMP WIRE0
    int STS35_ADDRESS = 0x4B; // I2C Address: either 0x4A or 0x04B

    // OUTTTER TEMP ANALOG
    int THERMISTOR_PIN = 26;
    float THERMISTOR_REFERENCE_R = 10000;
    float THERMISTOR_NOMINAL_R = 10000;
    float THERMISTOR_NOMINAL_T = 25;
    float THERMISTOR_B = -4050;

    // Battery voltage
    float BATT_SENS_CONVERSION_FACTOR = 3.3 * (1 / 0.3357);
    float BATT_SENS_PIN = 28;

    // HEATER
    int HEATER_MOSFET = 22; // mosfet 1

    // Parachute
    int PARACHUTE_MOSFET = 27;   // mosfet 2
    int LAUNCH_RAIL_SWITCH = -1; // TBD

    // detection parameters

    // ARMING AND DATA SENDING MSG IN PREP SATE
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String DATA_SEND_STOP_MSG = "data_stop";
    String HEATER_ENABLE_MSG = "heater_enable";
    String FORMAT_MSG = "format_flash";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};