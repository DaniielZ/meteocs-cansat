#include "states/descent_state.h"
void descent_state(Cansat &cansat)
{
    open_parachute(cansat);
    eject_nanosat(cansat);
    cansat.log.info("descent_state");

    while (true)
    {
        cansat.sensors.read_data();
        cansat.log.data(cansat);

        // check if should move to next state
        if (is_still())
        {
            return;
        }
    }
}

void open_parachute(Cansat &cansat)
{
}
void eject_nanosat(Cansat &cansat)
{
}

bool is_still()
{
}