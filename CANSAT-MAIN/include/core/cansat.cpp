#include "core/cansat.h"

void Cansat::start_states()
{
    current_state = State::PREP;
    prepare_state(*this);
    current_state = State::ARMED;
    armed_state(*this);
    current_state = State::ASCENT;
    ascent_state(*this);
    current_state = State::DESCENT;
    descent_state(*this);
    // will need ground state for flash health
}