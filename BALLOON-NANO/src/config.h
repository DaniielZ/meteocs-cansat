#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <LittleFS.h>
#include <SDFS.h>
#include "sensors/ranging_wrapper.h"
class Config
{
public:
    //------------ OFTEN CHANGED ------------------

    bool WAIT_PC = true;
    bool LOG_TO_STORAGE = true;

    Ranging_Wrapper::Ranging_Slave RANGING_SLAVES[3] = {{.position = {0, 0, 0}, .address = 0x12345671},
                                                        {.position = {0, 0, 0}, .address = 0x12345672},
                                                        {.position = {0, 0, 0}, .address = 0x12345673}};

    // LORA 433 SPI0
    Ranging_Wrapper::Lora_Device LORA433{
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
    Ranging_Wrapper::Lora_Device LORA2400{
        .FREQUENCY = 2405.6,
        .CS = 13,
        .DIO0 = 18,  // busy pin not programmable dont use
        .DIO1 = 15,  // only use thsi
        .RESET = 14, // 10
        .SYNC_WORD = 0xF5,
        .TXPOWER = 14,
        .SPREADING = 9,
        .CODING_RATE = 7,
        .SIGNAL_BW = 1600,
        .SPI = &SPI1};

    // Sea level Hpa for barometer height
    float SEA_LEVEL_HPA = 1015.0; // only used by outter baro

    float DESIRED_HEATER_TEMP = 35.0; // in C
    // float P = 0; // set in temp manager.h
    // float I = 0;
    // float D = 0;

    const int LORA_DATAPACKET_COOLDOWN = 5000; // in ms
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

    int RANGING_TIMEOUT = 200; // ms
    int SD_CARD_CS = 20;

    SPIClassRP2040 *SD_CARD_SPI = &SPI1;

    // BARO WIRE0
    int MS5611_ADDRESS = 0x76; // 77 or 76 (77 is the real one)

    int BMP180_ADDRESS_I2C = 0x77; // or 0x76 :D

    // INNER TEMP WIRE0
    int STS35_ADDRESS = 0x4A; // I2C Address: either 0x4A or 0x04B

    // OUTTTER TEMP ANALOG
    int THERMISTOR_PIN = -1;

    // HEATER
    int HEATER_MOSFET = 27; // TBD

    // Parachute
    int PARACHUTE_MOSFET = 26;   // TBD
    int LAUNCH_RAIL_SWITCH = -1; // TBD

    // hard data rate limiter
    const int MAX_LOOP_TIME = 5; // ms

    // detection parameters

    // ARMING AND DATA SENDING MSG IN PREP SATE
    String ARM_MSG = "arm_confirm";
    String DATA_SEND_MSG = "data_send";
    String FORMAT_MSG = "format_flash";
    String LOG_FILE_NAME_BASE_PATH = "/CANSAT";
};