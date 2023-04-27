#include "states/prepare_state.h"
void prepare_state(Cansat &cansat)
{
    cansat.log.init(cansat);
    cansat.sensors.init(cansat);
    cansat.log.info("init done, waiting for arm");
    //
    while (true)
    {
        // check for further commands
        String incoming_msg = "";
        cansat.log.read(cansat, incoming_msg);

        // check send data check
        if (incoming_msg == cansat.config.DATA_SEND_MSG)
        {
            cansat.sensors.read_data();
            cansat.log.data(cansat);
        }
        else if (incoming_msg == cansat.config.ARM_MSG)
        {
            cansat.log.info("Arming singal recieved");
            return;
        }
        else if (incoming_msg != "")
        {
            String noise_msg = "NOISE recieved: " + incoming_msg;
            cansat.log.info((char *)noise_msg.c_str());
        }

        delay(100);
    }
}
