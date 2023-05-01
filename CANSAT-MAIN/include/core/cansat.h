#pragma once
#include "config.h"
#include "states/armed_state.h"
#include "states/ascent_state.h"
#include "states/descent_state.h"
#include "states/prepare_state.h"
#include "states/landed_state.h"
#include "core/log.h"
#include "core/sound.h"
#include "sensors/sensor_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include "MS5611.h"
#include "Adafruit_SHTC3.h"
#include <Servo.h>
#include "LittleFS.h"

class Cansat
{
public:
    enum State
    {
        PREP,
        ARMED,
        ASCENT,
        DESCENT,
        LANDED
    };
    State current_state = State::PREP;

    Config config;
    Sensor_manager sensors;
    Log log;
    Sound sound;
    Servo ejection_servo;
    void start_states(Cansat &cansat);
};
