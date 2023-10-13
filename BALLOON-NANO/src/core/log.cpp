#include "core/log.h"
#include <SDFS.h>

volatile bool rfm_lora_transmiting = false; // yeah sadly wasnt able to move it into the class
void rfm_transmission_end(void)
{
    rfm_lora_transmiting = false;
}

void Log::init_lora(Ranging_Wrapper::Lora_Device &lora_cfg)
{
    // platform specific
    int state = _lora.begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("RFM lora failed state: ");
        Serial.println(state);
    }

    // setting paramaters
    _lora.setFrequency(lora_cfg.FREQUENCY);

    _lora.setOutputPower(lora_cfg.TXPOWER);
    _lora.setSpreadingFactor(lora_cfg.SPREADING);
    _lora.setCodingRate(lora_cfg.CODING_RATE);
    _lora.setBandwidth(lora_cfg.SIGNAL_BW);
    _lora.setSyncWord(lora_cfg.SYNC_WORD);
    // setup interupt
    _lora.setDio0Action(rfm_transmission_end, RISING);
    _lora_initialized = true;
}
bool Log::format_storage(Config &config)
{
    bool result = _flash->format();
    init_flash(config);
    return result;
}
void Log::init_flash(Config &config)
{
    if (!config.LOG_TO_STORAGE)
    {
        return;
    }
    _flash = config.FILE_SYSTEM;
    SDFSConfig sd_config;
    sd_config.setCSPin(config.SD_CARD_CS);
    sd_config.setSPI(*config.SD_CARD_SPI);
    _flash->setConfig(sd_config);
    // initilise flash
    if (_flash->begin())
    {
        Serial.println("FileSystem init success");
    }
    else
    {
        Serial.println("FileSystem init error");
    }

    // determine nr for final path
    int log_file_name_nr = 0;
    while (_flash->exists(config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt"))
    {
        log_file_name_nr++;
    }
    _log_file_path_final = config.LOG_FILE_NAME_BASE_PATH + String(log_file_name_nr) + ".txt";
    // print header
    File file = _flash->open(_log_file_path_final, "a+");
    file.println("DATA");
    if (!file)
    {
        Serial.println("Failed opening flash file");
    }
    file.close();
    Serial.println("Final path: " + _log_file_path_final);
    FSInfo64 fsinfo;
    _flash->info64(fsinfo);
    Serial.println("Current size:" + String((unsigned long)fsinfo.usedBytes / 1024) + "/" + String((unsigned long)fsinfo.totalBytes / 1024));

    _flash_initialized = true;
}

void Log::init(Config &config)
{
    init_flash(config);
    init_lora(config.LORA433);
    String status = " INIT STATUS: Flash ready?:" + String(_flash_initialized) + " | " + "Lora ready?:" + String(_lora_initialized);
    info(status);
}
void Log::info(String msg)
{
    // prints message to serial
    Serial.println(msg);
    // sends message over lora
    if (_lora_initialized)
    {
        while (rfm_lora_transmiting)
        {
            // Serial.println("waiting for lora ... ");
            delay(10);
        }
        _lora.finishTransmit();
        rfm_lora_transmiting = true;
        // Serial.println("size of packet:" + String(msg.length()));
        int state = _lora.startTransmit(msg);
        if (state != RADIOLIB_ERR_NONE)
        {
            rfm_lora_transmiting = false;

            Serial.println("Transmit error: " + String(state));
        }
        _lora.setDio0Action(rfm_transmission_end, RISING);
    }
    if (_flash_initialized)
    {
        // write to flash
        File file = _flash->open(_log_file_path_final, "a+");
        if (!file)
        {
            Serial.println("Failed opening flash file");
        }
        file.println(msg);
        file.close();
    }
}
void Log::data(Sensor_manager::Sensor_data &data, bool log_to_storage, bool transmit)
{
    String sendable_packet;
    String logable_packet;

    data_to_packet(data, sendable_packet, logable_packet);

    bool packet_sent = false;
    // sends data over lora if can be sent
    if (!rfm_lora_transmiting && _lora_initialized && transmit)
    {

        rfm_lora_transmiting = true;
        _lora.finishTransmit();
        // Serial.println("size of packet:" + String(packet.length()));

        rfm_lora_transmiting = true;
        int state = _lora.startTransmit(sendable_packet);
        if (state != RADIOLIB_ERR_NONE)
        {
            rfm_lora_transmiting = false;
            Serial.println("Transmit error: " + String(state));
        }
        else
        {
            packet_sent = true;
        }
        _lora.setDio0Action(rfm_transmission_end, RISING);
    }

    // prints data
    if (packet_sent)
    {
        Serial.print("SEND DATA: ");
    }
    else
    {
        Serial.print("NOT  DATA: ");
    }

    Serial.println(logable_packet);

    // logs data to flash if apropriate state
    if (log_to_storage && _flash_initialized)
    {
        File file = _flash->open(_log_file_path_final, "a+");
        if (!file)
        {
            Serial.println("File open failed");
        }

        file.println(logable_packet);
        file.close();
    }
}
void data_to_packet(Sensor_manager::Sensor_data &data, String &result_log, String &result_sent)
{
    String packet;
    packet += String(data.gps_lat, 6);
    packet += ", ";
    packet += String(data.gps_lng, 6);
    packet += ", ";
    packet += String(data.gps_height, 2);
    packet += ", ";
    packet += String(data.gps_sattelites);
    packet += ", ";
    packet += String(data.ranging_results[0].distance, 2);
    packet += ", ";
    packet += String(data.ranging_results[1].distance, 2);
    packet += ", ";
    packet += String(data.ranging_results[2].distance, 2);
    packet += ", ";
    packet += String(data.times_since_last_ranging_result[0]);
    packet += ", ";
    packet += String(data.times_since_last_ranging_result[1]);
    packet += ", ";
    packet += String(data.times_since_last_ranging_result[2]);
    packet += ", ";
    packet += String(data.ranging_position.lat, 6);
    packet += ", ";
    packet += String(data.ranging_position.lng, 6);
    packet += ", ";
    packet += String(data.ranging_position.height, 2);
    packet += ", ";
    packet += String(data.time_since_last_ranging_pos);
    packet += ", ";
    packet += String(data.inner_baro_pressure, 3);
    packet += ", ";
    packet += String(data.average_inner_temp, 2);
    packet += ", ";
    packet += String(data.average_outter_temp, 2);
    packet += ", ";
    packet += String(data.heater_power);
    packet += ", ";
    packet += String(data.acc[0], 4);
    packet += ", ";
    packet += String(data.acc[1], 4);
    packet += ", ";
    packet += String(data.acc[2], 4);
    packet += ", ";
    packet += String(data.time);

    result_sent = packet;

    packet += ", ";
    packet += String(data.gps_time);
    packet += ", ";
    packet += String(data.gyro[0]);
    packet += ", ";
    packet += String(data.gyro[1]);
    packet += ", ";
    packet += String(data.gyro[2]);
    packet += ", ";
    packet += String(data.outter_temp_thermistor);
    packet += ", ";
    packet += String(data.inner_baro_temp);
    packet += ", ";
    packet += String(data.inner_temp_probe);

    result_log = packet;
}
void Log::read(String &msg)
{
    if (!_lora_initialized)
    {
        return;
    }
    // wait for transmission end
    while (rfm_lora_transmiting)
    {
        delay(100);
        // Serial.println("Waiting for transmiter to end");
    }
    _lora.clearDio0Action();
    _lora.finishTransmit();

    int state = _lora.receive(msg);
    _lora.setDio0Action(rfm_transmission_end, RISING);

    if (state == RADIOLIB_ERR_NONE)
    {
        return;
    }

    msg = "";
}