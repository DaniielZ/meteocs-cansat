#include "states/prepare_state.h"
void prepare_state(Cansat &cansat)
{

    cansat.log.init(cansat.config);
    String status = cansat.sensors.init(cansat.config);
    cansat.log.info(status);
    // cansat.ejection_servo.attach(cansat.config.SERVO_PWM); // setup ejection servo
    // cansat.ejection_servo.write(cansat.config.SERVO_START_POS);
    cansat.log.info("init done, waiting for arm");
    cansat.log.info(cansat.sensors.header);
    while (true)
    {
        // check for further commands
        String incoming_msg = "";
        cansat.log.read(incoming_msg);
        // check send data check
        if (incoming_msg == cansat.config.DATA_SEND_MSG)
        {
            delay(2000);
            cansat.sensors.read_data(cansat.config);
            cansat.log.data(cansat.sensors.data, true);
        }
        // check if should arm
        else if (incoming_msg == cansat.config.ARM_MSG)
        {
            delay(2000);
            cansat.log.info("Arming singal recieved");
            return;
        }
        else if (incoming_msg != "")
        {
            delay(2000);
            String noise_msg = "NOISE recieved: " + incoming_msg;
            cansat.log.info((char *)noise_msg.c_str());
        }

        delay(cansat.config.SLEEP);
    }
}
