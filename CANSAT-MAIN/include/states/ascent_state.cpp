#include "states/ascent_state.h"

bool is_falling()
{
    // IDK man not gud
    // my proposal is being dependant on gps becausese baro wont work on drone(mybe).
    return false;
}

void ascent_state(Cansat &cansat)
{
    cansat.log.info("ascent_state");
    while (true)
    {
        cansat.sensors.read_data();
        cansat.log.data(cansat);

        // check if falling
        if (is_falling())
        {
            return;
        }
        delay(100);
    }
    return;
}
