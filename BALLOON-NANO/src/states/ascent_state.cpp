#include "states/ascent_state.h"

unsigned long int last_data_transmit_time_ascent = 0;
unsigned long int last_state_save_time_ascent = 0;

// HELPER FUNCTIONS
void send_data_ascent(Cansat &cansat)
{
    // Print data to serial
    cansat.log.log_telemetry_data_to_pc();
    // Save data to telemetry file
    cansat.log.log_telemetry_data();

    // Check if data should be sent over LoRa
    if (millis() - last_data_transmit_time_ascent >= cansat.config.LORA_DATAPACKET_COOLDOWN_ASCENT)
    {
        // Send data by LoRa
        cansat.log.transmit_data(cansat.config);
        last_data_transmit_time_ascent = millis();
    }
}


// MAIN FUNCTIONS
// Ascent state loop
bool ascent_state_loop(Cansat &cansat)
{
    int launch_rail_removed_cycle_count = 0;

    unsigned long loop_start = millis();

    String incoming_msg = cansat.receive_command(cansat);

    // Check if message is a command
    // Check if should reset to PREP state
    if (incoming_msg == cansat.config.RESET_STATE_MSG)
    {
        cansat.config.last_state_variables.last_state = 0;
        cansat.save_last_state(cansat);
    }
    // FOR TESTING PURPOSES
    if (incoming_msg == "set_descent")
    {
        cansat.log.send_info("DESCENT state activated manually", cansat.config);
        return true;
    }

    // Read data from sensors
    cansat.sensors.read_data(cansat.log, cansat.config);

    // Log/Send data
    send_data_ascent(cansat);

    // Save data to last state
    if (millis() - last_state_save_time_ascent >= cansat.config.ASCENT_STATE_SAVE_UPDATE_INTERVAL)
    {
        cansat.config.last_state_variables.last_inner_temp = cansat.sensors.data.average_inner_temp;
        cansat.config.last_state_variables.last_integral_term = cansat.sensors._temp_manager->_integral_term;
        cansat.config.last_state_variables.last_safe_temp = cansat.sensors._temp_manager->_safe_temp;
        cansat.save_last_state(cansat);
    }

    // Used for debugging
    // Serial.println("Used heap: " + String(rp2040.getUsedHeap()));

    /*
    // Currently not really functional, because a digital pin is required for it to not throw an error
    if (digitalRead(cansat.config.LAUNCH_RAIL_SWITCH))
    {
        launch_rail_removed_cycle_count++;
        if (launch_rail_removed_cycle_count >= 10)
        {
            cansat.log.send_info("Launch rail removed", cansat.config);
            return true;
        }
    }
    else
    {
        launch_rail_removed_cycle_count = 0;
    }
    */

    // Check if should wait before starting next loop
    unsigned long loop_time = millis() - loop_start;
    if (loop_time < cansat.config.MAX_LOOP_TIME)
    {
        delay(cansat.config.MAX_LOOP_TIME - loop_time);
    }
    return false;
}

// Ascent state setup
void ascent_state(Cansat &cansat)
{
    // If payload has recovered to ascent state
    if (cansat._has_recovered_to_state && cansat.config.last_state_variables.last_state == 1)
    {
        // Init sensors
        String status = String("Sensor status: ") + cansat.sensors.init(cansat.log, cansat.config);
        cansat.log.send_info(status, cansat.config);

        cansat.log.send_info("Reset done", cansat.config);
    }

    // Run prepare loop while waiting for arming signal
    while (!ascent_state_loop(cansat))
    {
    }
}
