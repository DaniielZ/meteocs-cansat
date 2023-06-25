#include "states/landed_state.h"
void landed_state(Cansat &cansat)
{
    cansat.log.info("landed_state");
    while (true)
    {
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, false);
        cansat.sound.locate(cansat.config);
        delay(cansat.config.MAX_LOOP_TIME);
    }
}