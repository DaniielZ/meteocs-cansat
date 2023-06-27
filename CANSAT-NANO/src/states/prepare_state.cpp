#include "states/prepare_state.h"
void prepare_state(Cansat &cansat)
{
    cansat.log.init(cansat.config);
    String status = String("sensor status ") + cansat.sensors.init(cansat.config);
    cansat.log.info(status);
    cansat.ejection_servo.attach(cansat.config.SERVO_PWM); // setup ejection servo
    cansat.ejection_servo.writeMicroseconds(cansat.config.SERVO_START);
    bool servo_high = false;

    cansat.log.info("init done, waiting for arm");
    //   cansat.log.info(cansat.sensors.header);
    cansat.sound.init_success(cansat.config);

    while (true)
    {
        unsigned long loop_start = millis();

        // check for further commands either pc or lora
        String incoming_msg = "";
        cansat.log.read(incoming_msg);
        // can be overwriten by computer
        if (Serial.available() > 0)
        {
            incoming_msg = Serial.readString();
        }
        // read sensor data
        cansat.sensors.read_data(cansat.config);
        // cansat.log.data(cansat.sensors.data, true);

        // check send data check
        if (incoming_msg == cansat.config.DATA_SEND_MSG)
        {
            for (int i = 0; i < 10; i++)
            {
                cansat.sensors.read_data(cansat.config);
                cansat.log.data(cansat.sensors.data, true);
                delay(1000);
            }
        }
        // check if should arm
        else if (incoming_msg == cansat.config.ARM_MSG)
        {
            cansat.log.info("Arming singal recieved");
            return;
        }
        else if (incoming_msg == cansat.config.SERVO_MSG)
        {
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
        else if (incoming_msg == cansat.config.FORMAT_MSG)
        {
            if (LittleFS.format())
            {
                cansat.log.info("Formating done");
            }
            else
            {
                cansat.log.info("Formating fail");
            }
        }
        else if (incoming_msg != "")
        {
            String noise_msg = "NOISE recieved: " + incoming_msg;
            cansat.log.info((char *)noise_msg.c_str());
        }

        // check if should wait before next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            Serial.println("Waiting " + String(cansat.config.MAX_LOOP_TIME - loop_time));
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
