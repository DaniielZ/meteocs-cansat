#include "states/descent_state.h"
void descent_state(Cansat &cansat)
{
    cansat.log.info("descent_state");
    eject_nanosat(cansat);
    toggle_parachute(cansat, HIGH);

    while (true)
    {
        cansat.sensors.read_data(cansat);
        cansat.log.data(cansat);

        // check if should move to next state
        if (is_still(cansat))
        {
            toggle_parachute(cansat, LOW);
            return;
        }

        delay(100);
    }
}

void toggle_parachute(Cansat &cansat, PinStatus mosfet_state)
{
    digitalWrite(cansat.config.MOSFET, mosfet_state);
}
void eject_nanosat(Cansat &cansat)
{
    // open servo
    cansat.ejection_servo.write(cansat.config.SERVO_END_POS);
}

bool is_still(Cansat &cansat)
{
    return true;
}