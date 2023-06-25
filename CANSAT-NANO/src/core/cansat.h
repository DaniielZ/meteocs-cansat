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
#include <Servo.h>

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
