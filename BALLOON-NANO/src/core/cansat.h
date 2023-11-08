#pragma once
#include "config.h"
#include "states/ascent_state.h"
#include "states/descent_state.h"
#include "states/prepare_state.h"
#include "core/log.h"
#include "sensors/sensor_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

// Cansat object
class Cansat
{
public:
    // Available Cansat states
    enum State
    {
        PREP,
        ASCENT,
        DESCENT,
    };

    State current_state = State::PREP;

    Config config;
    Sensor_manager sensors;
    Log log;

    // Function declaration
    void init_all_com_bus(Config &config);
    void start_states(Cansat &cansat);
};
