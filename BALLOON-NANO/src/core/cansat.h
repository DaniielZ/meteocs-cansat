#pragma once
#include "config.h"
#include "states/armed_state.h"
#include "states/prepare_state.h"
#include "core/log.h"
#include "core/sound.h"
#include "sensors/sensor_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

class Cansat
{
public:
    enum State
    {
        PREP,
        ARMED,
    };

    State current_state = State::PREP;

    Config config;
    Sensor_manager sensors;
    Log log;
    Sound sound;
    Servo ejection_servo;
    void init_all_com_bus(Config &config);
    void start_states(Cansat &cansat);
};
