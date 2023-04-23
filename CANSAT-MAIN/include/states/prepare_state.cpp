#include "states/prepare_state.h"
void prepare_state(Cansat &cansat)
{
    cansat.log.init(cansat);
    cansat.sensors.init(cansat);

    //
    while (true)
    {
        // check for further commands
    }
}
