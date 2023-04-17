#pragma once
#include "config.h"
#include "states/armed_state.h"
#include "states/ascent_state.h"
#include "states/descent_state.h"
#include "states/prepare_state.h"
#include "core/log.h"
#include "sensors/sensor_manager.h"

#include <Arduino.h>

class Cansat
{
public:
    enum State
    {
        PREP,
        ARMED,
        ASCENT,
        DESCENT
    };
    State current_state = State::PREP;

    Config config;
    Sensor_manager sensors;
    Log log;
    void start_states();
};