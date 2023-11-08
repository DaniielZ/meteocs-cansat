#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <LittleFS.h>
#include <SDFS.h>
#include <ranging_wrapper.h>
#include <RFM_Wrapper.h>

class Config
{
public:
    //------------ OFTEN CHANGED ------------------
    // hard data rate limiter
    const int MAX_LOOP_TIME = 20; // ms

    bool WAIT_PC = false;
    bool LOG_TO_STORAGE = true;
    
    // 433 MHz LoRa
    RFM_Wrapper com_lora;
    RFM_Wrapper::Lora_Device com_config = {.FREQUENCY = 434.5,
                                        .CS = 5,
                                        .DIO0 = 7,
                                        .DIO1 = 8,
                                        .RESET = 6,
                                        .SYNC_WORD = 0xF4,
                                        .TXPOWER = 14,
                                        .SPREADING = 10,
                                        .CODING_RATE = 7,
                                        .SIGNAL_BW = 125,
                                        .SPI = &SPI1};

    // Ranging 2.4 GHZ LoRa
    
    Ranging_Wrapper::Ranging_Slave RANGING_SLAVES[3] = {{.position = {0, 0, 0}, .address = 0x123456},
                                                        {.position = {0, 0, 0}, .address = 0xABCD98},
                                                        {.position = {0, 0, 0}, .address = 0x9A8B7C}};
    
    Ranging_Wrapper::Mode LORA2400_MODE = Ranging_Wrapper::Mode::SLAVE;
    Ranging_Wrapper::Lora_Device ranging_device = {.FREQUENCY = 2405.6,
                                                .CS = 13,
                                                .DIO0 = 18, // busy
                                                .DIO1 = 15,
                                                .RESET = 14,
                                                .SYNC_WORD = 0xF5,
                                                .TXPOWER = 14,
                                                .SPREADING = 10,
                                                .CODING_RATE = 7,
                                                .SIGNAL_BW = 406.25,
                                                .SPI = &SPI};
    

    float HEATER_CUT_OFF_VOLTAGE = 6.0; // V
    float DESIRED_HEATER_TEMP = 35.0;   // in C
    unsigned int OUTER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int OUTER_TEMP_AVERAGE_CAPACITY = ((OUTER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    unsigned int INNER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int INNER_TEMP_AVERAGE_CAPACITY = ((INNER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    unsigned int BAT_AVERAGE_TIME = 5000;
    const unsigned int BAT_AVERAGE_CAPACITY = ((BAT_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);

    const int LORA_DATAPACKET_COOLDOWN = 30000; // in ms  30000
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

    int RANGING_TIMEOUT = 500; // ms  might need to adjust this if lora parameters change
    int SD_CARD_CS = 20;

    SPIClassRP2040 *SD_CARD_SPI = &SPI;

    int BMP180_ADDRESS_I2C = 0x77; // or 0x76 :D

    // INNER TEMP WIRE0
    int STS35_ADDRESS = 0x4B; // I2C Address: either 0x4A or 0x04B

    // OUTER TEMP ANALOG
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
    String TELEMETRY_LOG_FILE_NAME_BASE_PATH = "/CANSAT_TELEMETRY";
    String INFO_LOG_FILE_NAME_BASE_PATH = "/CANSAT_INFO";
    String ERROR_LOG_FILE_NAME_BASE_PATH = "/CANSAT_ERROR";

    String TELEMETRY_HEADER = "gps_lat,gps_lng,gps_height,gps_satellites,r1_dist,r2_dist,r3_dist,r1_time_since,r2_time_since,r3_time_since,r_pos_lat,r_pos_lng,r_pos_time_since,inner_pressure,avg_inner_temp,avg_outer_temp,heater_power,acc_x,acc_y,acc_z,gps_time_since,time_on,avg_batt_voltage,gps_time,gyro_x,gyro_y,gyro_z,outer_temp_thermistor,raw_inner_temp_baro,raw_inner_temp_probe,batt_voltage,p_term,i_term,d_term,target_temp";
    String INFO_HEADER = "time_on, info";
    String ERROR_HEADER = "time_on,error";
};