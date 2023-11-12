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

    const bool WAIT_PC = true;
    const bool LOG_TO_STORAGE = true;
    
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
                                        .SPI = &SPI};

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
                                                .SPI = &SPI1};
    
    const float HEATER_CUT_OFF_VOLTAGE = 6.0; // V
    const float DESIRED_HEATER_TEMP = 35.0;   // in C
    const unsigned int OUTER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int OUTER_TEMP_AVERAGE_CAPACITY = ((OUTER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    const unsigned int INNER_TEMP_AVERAGE_TIME = 3000;
    const unsigned int INNER_TEMP_AVERAGE_CAPACITY = ((INNER_TEMP_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);
    const unsigned int BAT_AVERAGE_TIME = 5000;
    const unsigned int BAT_AVERAGE_CAPACITY = ((BAT_AVERAGE_TIME / MAX_LOOP_TIME) * 1.5);

    const int LORA_DATAPACKET_COOLDOWN = 30000; // in ms  30000
    const int TIME_FROM_LAUNCH_TO_EJECT = 20000;     // ms
    const int MOSFET_ON_TIME = 10000;                // ms

    //----------------------------------SOMEWHAT STATIC -------------------------------------

    // logging
    const unsigned long PC_BAUDRATE = 115200;
    FS *FILE_SYSTEM = &SDFS; // if change to LittleFS need to change some code

    // GPS UART0
    const int SERIAL1_RX = 17;
    const int SERIAL1_TX = 16;
    const long SERIAL1_BAUDRATE = 9600;
    
    // Wire0
    const int WIRE0_SCL = 1;
    const int WIRE0_SDA = 0;
    
    // SPI0
    const int SPI0_RX = 4;
    const int SPI0_TX = 3;
    const int SPI0_SCK = 2;
    
    // SPI1
    const int SPI1_RX = 12;
    const int SPI1_TX = 11;
    const int SPI1_SCK = 10;

    const int RANGING_TIMEOUT = 500; // ms  might need to adjust this if lora parameters change
    const int SD_CARD_CS = 20;

    SPIClassRP2040 *SD_CARD_SPI = &SPI;

    const int MS5611_ADDRESS_I2C = 0x76; 
    const int BMP180_ADDRESS_I2C = 0x77; // or 0x76 :D

    // INNER TEMP WIRE0
    const int STS35_ADDRESS = 0x4B; // I2C Address: either 0x4A or 0x04B

    // OUTER TEMP ANALOG
    const int THERMISTOR_PIN = 26;
    const float THERMISTOR_REFERENCE_R = 10000;
    const float THERMISTOR_NOMINAL_R = 10000;
    const float THERMISTOR_NOMINAL_T = 25;
    const float THERMISTOR_B = -4050;

    // Battery voltage
    const float BATT_SENS_CONVERSION_FACTOR = 3.3 * (1 / 0.3357);
    const float BATT_SENS_PIN = 28;

    // HEATER
    const int HEATER_MOSFET = 22; // mosfet 1

    // Parachute
    const int PARACHUTE_MOSFET = 27;   // mosfet 2
    const int LAUNCH_RAIL_SWITCH = 0; // TBD

    // SENSOR FAILED STATE EEPROM ADDRESSES
    int OUTER_BARO_FAILED_EEPROM_ADDRESS = 100;
    int INNER_BARO_FAILED_EEPROM_ADDRESS = 101;
    int INNER_TEMP_PROBE_FAILED_EEPROM_ADDRESS = 102;
    int IMU_FAILED_EEPROM_ADDRESS = 103;
    int OUTER_THERMISTOR_FAILED_EEPROM_ADDRESS = 104;
    int RANGING_LORA_FAILED_EEPROM_ADDRESS = 105;

    // SENSOR TIMEOUT CONSTANTS (milliseconds)
    const unsigned int OUTER_BARO_TIMEOUT = 100;
    const unsigned int INNER_BARO_TIMEOUT = 100;
    const unsigned int INNER_TEMP_PROBE_TIMEOUT = 100;
    const unsigned int IMU_TIMEOUT = 200;
    const unsigned int OUTER_THERMISTOR_TIMEOUT = 100;
    const unsigned int RANGING_LORA_TIMEOUT = 200;

    // Max allowed unsuccessful readings
    const int OUTER_BARO_MAX_ATTEMPTS = 3;
    const int INNER_BARO_MAX_ATTEMPTS = 3;
    const int INNER_TEMP_PROBE_MAX_ATTEMPTS = 3;
    const int IMU_MAX_ATTEMPTS = 3;
    const int OUTER_THERMISTOR_MAX_ATTEMPTS = 3;

    // SENSOR VALUE ACCEPTABLE RANGES
    // Outer baro (Datasheet operational ranges)
    const float OUTER_BARO_MIN_PRESSURE = 1000;
    const float OUTER_BARO_MAX_PRESSURE = 120000;

    // Inner baro (Datasheet operational ranges)
    const float INNER_BARO_MIN_PRESSURE = 30000;
    const float INNER_BARO_MAX_PRESSURE = 110000;
    const float INNER_BARO_MIN_TEMP = -40;
    const float INNER_BARO_MAX_TEMP = 85;

    // Inner temperature probe
    const float INNER_TEMP_PROBE_MIN_TEMP = -40;
    const float INNER_TEMP_PROBE_MAX_TEMP = 60; // Safe assumption of max temp inside. If it goes higher something has gone horribly wrong

    // IMU 
    // NOT SURE IF ON IMPACT THE ACC COULD BE HIGHER???
    const float IMU_MIN_ACCELERATION = -100;
    const float IMU_MAX_ACCELERATION = 100; 
    // Probably won't be rotating faster that this, right??? 
    const float IMU_MIN_ROTATION = -36000; // 100 Hz rotations (360 deg*100)
    const float IMU_MAX_ROTATION = 36000;

    // Battery voltage
    const float BATT_VOLTAGE_MIN = 0;
    const float BATT_VOLTAGE_MAX = 8;

    // Outer thermistor
    const float OUTER_THERMISTOR_MIN_TEMP = -100;
    const float OUTER_THERMISTOR_MAX_TEMP = 100;
    
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