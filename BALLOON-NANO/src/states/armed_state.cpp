#include "states/armed_state.h"
#include "core/gnc_math.h"
#include <Array.h>

void armed_state(Cansat &cansat)
{

    // 10 sec buffer KINDA well its just 50 data pounts but will definetly work for this example
    Array<Sensor_manager::Sensor_data, 30> data_buffer;
    cansat.sensors.read_data(cansat.config);

    float starting_height = cansat.sensors.data.baro_height;
    cansat.log.info("REFERENCE HEIGHT: " + String(starting_height, 1) + "m");

    while (true)
    {

        unsigned long loop_start = millis();
        cansat.sensors.read_data(cansat.config);
        cansat.log.data(cansat.sensors.data, false);

        data_buffer.push_back(cansat.sensors.data);
        // remove unneeded old data
        while (data_buffer.size() > 25)
        {
            data_buffer.remove(0);
        }
        // launch detection
        if (data_buffer.size() > cansat.config.DATA_POINTS_FOR_LAUNCH_DETECTION)
        {
            bool all_values_over_threshold = true;
            float height_delta_sum = 0;
            int i = data_buffer.size() - 1;
            int i_last = i - cansat.config.DATA_POINTS_FOR_LAUNCH_DETECTION;
            while (i > i_last)
            {
                float delta_height = data_buffer[i].baro_height - starting_height;
                if (delta_height < cansat.config.LAUNCH_DETECTION_HEIGHT)
                {
                    all_values_over_threshold = false;
                    break;
                }
                i--;
            }
            if (all_values_over_threshold) // || data_buffer.size() > 23
            {
                // launch detected
                // log buffer
                cansat.log.info("LAUNCH DETECTED");
                unsigned long time_at_log_start = millis();
                for (Sensor_manager::Sensor_data data : data_buffer)
                {
                    cansat.log.data(data);
                }
                unsigned long time_to_log = millis() - time_at_log_start;
                cansat.log.info("BUFFER LOGGED: " + String(time_to_log));
                data_buffer.clear();
                return;
            }
        }

        // check if should wait before next loop
        unsigned long loop_time = millis() - loop_start;
        if (loop_time < cansat.config.MAX_LOOP_TIME)
        {
            delay(cansat.config.MAX_LOOP_TIME - loop_time);
        }
    }
}
