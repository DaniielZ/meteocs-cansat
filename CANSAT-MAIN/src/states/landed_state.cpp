#include "states/landed_state.h"
void landed_state(Cansat &cansat)
{
    cansat.log.info("landed_state");
    while (true)
    {
        cansat.sensors.read_data();
        cansat.log.data(cansat);
        cansat.sound.locate(cansat);
        delay(100);
    }
}