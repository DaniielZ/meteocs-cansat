#include "states/ascent_state.h"

unsigned long int last_data_transmit_time_ascent = 0;

void send_data_ascent(Cansat &cansat)
{
    // Print data to serial
    cansat.log.log_data_to_pc();
    // Save data to telemetry file
    cansat.log.log_data_to_flash();

    // Check if data should be sent over LoRa
    if (millis() > last_data_transmit_time_ascent + cansat.config.LORA_DATAPACKET_COOLDOWN)
    {
        // Send data by LoRa
        cansat.log.transmit_data(cansat.config);
        last_data_transmit_time_ascent = millis();
    }
}

void ascent_state(Cansat &cansat)
{
    int launch_rail_removed_cycle_count = 0;

    while (true)
    {
        unsigned long loop_start = millis();

        // Read data from sensors
        cansat.sensors.read_data(cansat.config);
        // Update data packets
        cansat.log.update_data_packet(cansat.sensors.data, cansat.log._sendable_packet, cansat.log._loggable_packet);
        // Log/Send data
        send_data_ascent(cansat);
        // Used for debugging
        Serial.println("Used heap: " + String(rp2040.getUsedHeap()));

        if (digitalRead(cansat.config.LAUNCH_RAIL_SWITCH))
        {
            launch_rail_removed_cycle_count++;
            if (launch_rail_removed_cycle_count >= 10)
            {
                cansat.log.send_info("Launch rail removed", cansat.config);
                return;
            }
        }
        else
        {
            launch_rail_removed_cycle_count = 0;
        }

        // Check if should wait before starting next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
