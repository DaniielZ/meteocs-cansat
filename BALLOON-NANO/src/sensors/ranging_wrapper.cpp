#include "ranging_wrapper.h"

volatile bool sx1280_lora_ranging = false; // can't be moved inside class. Shows if lora is actively doing ranging
void sx1280_ranging_end(void)
{
    sx1280_lora_ranging = false;
}

String Ranging_Wrapper::init(Mode mode, Lora_Device config)
{
    _lora = new Module(config.CS, config.DIO1, config.RESET, config.DIO0, *config.SPI);
    _mode = mode;
    _config = config;

    String status;
    int state = _lora.begin();
    if (state != RADIOLIB_ERR_NONE)
    {
        status += " SX1280 error (" + String(state) + ")";
    }
    else
    {
        // setting paramaters
        _lora.setOutputPower(_config.TXPOWER);
        _lora.setSpreadingFactor(_config.SPREADING);
        _lora.setCodingRate(_config.CODING_RATE);
        _lora.setBandwidth(_config.SIGNAL_BW);
        _lora.setSyncWord(_config.SYNC_WORD);
        _lora.setFrequency(_config.FREQUENCY);
        _lora_initialized = true;
    }

    return status;
}
bool Ranging_Wrapper::master_read(Ranging_Slave slave, Ranging_Result &result, long int timeout)
{
    bool result_read = false;
    if (_mode != Mode::MASTER || !_lora_initialized)
    {
        return result_read;
    }

    if (sx1280_lora_ranging)
    {
        // check if should timeout
        if (millis() >= _ranging_start_time + timeout)
        {
            sx1280_lora_ranging = false;
            _lora_range_state = RADIOLIB_ERR_RANGING_TIMEOUT;
            _lora.clearDio1Action();
            _lora.finishTransmit();
        }
        return result_read;
    }
    if (!sx1280_lora_ranging)
    {

        // if available read result
        if (_lora_range_state == RADIOLIB_ERR_NONE)
        {
            result.distance = _lora.getRangingResult();
            result.time = millis();
            result_read = true;
        }
        // clean up
        _lora.clearDio1Action();
        _lora.finishTransmit();
        _lora_range_state = -1;

        // start ranging but first reset lora
        _lora.reset();
        init(_mode, _config);

        // setup interrupt
        _lora.setDio1Action(sx1280_ranging_end);
        sx1280_lora_ranging = true;
        _lora_range_state = _lora.startRanging(true, slave.address);
        _ranging_start_time = millis();
        if (_lora_range_state != RADIOLIB_ERR_NONE)
        {
            Serial.println("Ranging error");
        }
    }
    return result_read;
}
void Ranging_Wrapper::slave_reenable()
{
    if (_mode != Mode::SLAVE || !_lora_initialized)
    {
        return;
    }
}
bool Ranging_Wrapper::trilaterate_position(Ranging_Result readings[3], Ranging_Slave slaves[3], Positon &result)
{
    Positon calculated_pos;
    // set global space
    Positon global_bs_pos[3];

    for (int i = 0; i < 3; i++)
    {
        global_bs_pos[i] = slaves[i].position;
    }

    // global space to local space
    Positon local_bs_pos[3];

    // check if data is usable if not return false;

    // calculate pos in local space

    // local space to global space

    // set results

    result = calculated_pos;
    return true;
}
