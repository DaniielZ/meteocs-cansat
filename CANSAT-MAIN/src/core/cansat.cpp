#include <core/cansat.h>

void Cansat::start_states(Cansat &cansat)
{
    current_state = State::PREP;
    prepare_state(cansat);
    // current_state = State::ARMED;
    // armed_state(cansat);
    // current_state = State::ASCENT;
    // ascent_state(cansat);
    current_state = State::DESCENT;
    descent_state(cansat);
    current_state = State::LANDED;
    landed_state(cansat);
}
