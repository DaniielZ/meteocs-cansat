#include "states/prepare_state.h"
#include <SDFS.h>

bool prepare_state_loop(Cansat &cansat)
{
    unsigned long loop_start = millis();
    
    // Check for further commands either from PC or LoRa
    String incoming_msg = "";
    float rssi;
    float snr;
    cansat.log.receive_main_lora(incoming_msg, rssi, snr, cansat.config);
    if (Serial.available() > 0)
    {
        incoming_msg = Serial.readString();
    }

    // Remove any new line characters and log the received message
    if (incoming_msg != "")
    {
        incoming_msg.trim();
        Serial.println(incoming_msg + ", " + String(rssi) + ", " + String(snr));
        cansat.log.send_info("Payload received message: " + incoming_msg, cansat.config);
    }

    // Read sensor data
    cansat.sensors.read_data(cansat.log, cansat.config);

    // Check received message
    // Check if should send telemetry data for a short moment
    if (incoming_msg == cansat.config.DATA_SEND_MSG)
    {
        unsigned long data_send_start_time = millis();
        unsigned long data_send_end_time = data_send_start_time + 30000;  // 30 seconds 
        while (millis() <= data_send_end_time)
        {
            // Get sensor data
            cansat.sensors.read_data(cansat.log, cansat.config);
            // Print data to serial
            cansat.log.log_data_to_pc();
            // Save data to telemetry file
            cansat.log.log_data_to_flash();
            // Send data by LoRa
            cansat.log.transmit_data(cansat.config);

            delay(cansat.config.MAX_LOOP_TIME);

            // Check for any commands from PC or LoRa
            cansat.log.receive_main_lora(incoming_msg, rssi, snr, cansat.config);
            if (Serial.available() > 0)
            {
                incoming_msg = Serial.readString();
            }

            if (incoming_msg == cansat.config.DATA_SEND_STOP_MSG)
            {
                break;
            }
        }
    }
    // Check if should enable heater
    else if (incoming_msg == cansat.config.HEATER_ENABLE_MSG)
    {
        cansat.sensors.set_heater(true);
    }
    // Check if should arm
    else if (incoming_msg == cansat.config.ARM_MSG)
    {
        cansat.log.send_info("Arming signal received", cansat.config);
        return true;
    }
    // Check if should format SD card
    else if (incoming_msg == cansat.config.FORMAT_MSG)
    {
        if (cansat.log.format_storage(cansat.config))
        {
            cansat.log.send_info("Formatting done", cansat.config);
        }
        else
        {
            cansat.log.send_info("Formatting fail", cansat.config);
        }
    }
    // If message doesn't match any case
    else if (incoming_msg != "")
    {
        String noise_msg = "NOISE received: " + incoming_msg;
        cansat.log.send_info(noise_msg, cansat.config);
    }

    // Check if should wait before next loop
    unsigned long loop_time = millis() - loop_start;
    if (loop_time < cansat.config.MAX_LOOP_TIME)
    {
        // Serial.println("Waiting " + String(cansat.config.MAX_LOOP_TIME - loop_time));
        delay(cansat.config.MAX_LOOP_TIME - loop_time);
    }
    return false;
}

// Prepare state start
void prepare_state(Cansat &cansat)
{
    // Init communications
    cansat.init_all_com_bus(cansat.config);

    // Init logging
    cansat.log.init(cansat.config);
    
    // Init sensors
    String status = String("Sensor status: ") + cansat.sensors.init(cansat.log, cansat.config);
    cansat.log.send_info(status, cansat.config);

    cansat.log.send_info("Init done, waiting for arm", cansat.config);

    // Run prepare loop while waiting for arming signal
    while (!prepare_state_loop(cansat))
    {
    }
}
