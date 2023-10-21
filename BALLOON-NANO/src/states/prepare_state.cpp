#include "states/prepare_state.h"
#include <SDFS.h>

void prepare_state_loop(Cansat &cansat)
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
    // check send data check
    if (incoming_msg == cansat.config.DATA_SEND_MSG)
    {
        unsigned long data_send_start_time = millis();
        unsigned long data_send_end_time = data_send_start_time + 1800000;
        while (millis() <= data_send_end_time)
        {
            cansat.sensors.read_data(cansat.config);
            cansat.log.data(cansat.sensors.data, true, true);
            delay(cansat.config.MAX_LOOP_TIME);
        }
    }
    // check if should arm
    else if (incoming_msg == cansat.config.ARM_MSG)
    {
        cansat.log.info("Arming singal recieved");
        return;
    }
    else if (incoming_msg == cansat.config.FORMAT_MSG)
    {
        if (cansat.log.format_storage(cansat.config))
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

void prepare_state(Cansat &cansat)
{
    cansat.init_all_com_bus(cansat.config);
    cansat.log.init(cansat.config);
    String status = String("sensor status ") + cansat.sensors.init(cansat.config);
    cansat.log.info(status);
    cansat.log.info("init done, waiting for arm");

    while (true)
    {
        prepare_state_loop(cansat);
    }
}
