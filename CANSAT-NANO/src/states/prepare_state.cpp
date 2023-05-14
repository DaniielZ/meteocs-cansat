#include "states/prepare_state.h"
void prepare_state(Cansat &cansat)
{

    cansat.log.init(cansat.config);
    String status = cansat.sensors.init(cansat.config);
    cansat.log.info(status);
    cansat.ejection_servo.attach(cansat.config.SERVO_PWM); // setup ejection servo
    cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_START);
    bool servo_high = false;
    cansat.log.info("init done, waiting for arm");
    cansat.log.info(cansat.sensors.header);
    cansat.sound.init_success(cansat.config);
    while (true)
    {
        cansat.sensors.read_data(cansat.config); // needs to be run for gps
        // check for further commands either pc or lora
        String incoming_msg = "";

        cansat.log.read(incoming_msg);
        // can be overwriten by computer
        if (Serial.available() > 0)
        {
            incoming_msg = Serial.readString();
        }
        // check send data check
        if (incoming_msg == cansat.config.DATA_SEND_MSG)
        {
            delay(2000);
            // cansat.sensors.read_data(cansat.config);
            cansat.log.data(cansat.sensors.data, true);
        }
        // check if should arm
        else if (incoming_msg == cansat.config.ARM_MSG)
        {
            delay(2000);
            cansat.log.info("Arming singal recieved");
            return;
        }
        // check if
        else if (incoming_msg == cansat.config.SERVO_MSG)
        {
            delay(2000);
            cansat.log.info("Servo singal recieved");
            if (servo_high == true)
            {
                cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_START);
                servo_high = false;
            }
            else
            {
                cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_END);
                servo_high = true;
            }
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
