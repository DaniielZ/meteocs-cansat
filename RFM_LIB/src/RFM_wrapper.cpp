#include "RFM_wrapper.h"

volatile bool action_done = false;

// Set the flag if sent or received a packet
void RFM_Wrapper::set_flag(void)
{
    action_done = true;
}

bool RFM_Wrapper::get_flag()
{
    return action_done;
}

// Return LoRa initialization status
bool RFM_Wrapper::get_init_status()
{
    return _lora_initialized;
}

// Initialize LoRa module
String RFM_Wrapper::init(bool transmit_first, Lora_Device config)
{   
    // Create new LoRa object
    _lora = new Module(config.CS, config.DIO0, config.RESET, config.DIO1, *config.SPI);
    
    // Try to initialize communication with LoRa
    int status = _lora.begin();
    
    // If initialization failed, print error
    if (status != RADIOLIB_ERR_NONE)
    {
        return "RFM lora failed state: " + String(status);
    }

    // Set LoRa parameters
    _lora.setFrequency(config.FREQUENCY);
    _lora.setOutputPower(config.TXPOWER);
    _lora.setSpreadingFactor(config.SPREADING);
    _lora.setCodingRate(config.CODING_RATE);
    _lora.setBandwidth(config.SIGNAL_BW);
    _lora.setSyncWord(config.SYNC_WORD);

    // Set interrupt behaviour
    _lora.setDio0Action(set_flag, RISING);

    if (transmit_first)
    {
        // Try to transmit an successful initialization message
        int _action_state = _lora.startTransmit("LoRa Init");

        // If transmit failed, print error
        if (_action_state != RADIOLIB_ERR_NONE)
        {
            _lora_initialized = false;
            return "Transmit starting failed:" + String(_action_state);
        }
        else
        {
            _last_action_transmit = true;
            Serial.println("Transmit started");
            Serial.print("Waiting .");

            while (!action_done)
            {
                delay(25);
                Serial.print(".");
            }

            if (_action_state == RADIOLIB_ERR_NONE)
            {
                _lora_initialized = true;
                Serial.println(" Done");
                return "Transmit Good";
            }
            else
            {
                Serial.println(" Something went wrong: " + String(_action_state));
                return "Transmit Bad : " + String(_action_state);
            }
        }
    }
    else
    {
        int receive_state = _lora.startReceive();
        if (receive_state != RADIOLIB_ERR_NONE)
        {
            _lora_initialized = false;
            return "Receive starting failed:" + String(receive_state);
        }
        else
        {
            _lora_initialized = true;
            _last_action_transmit = false;
            return "Receive started";
        }
    }
}

// Send message over LoRa. Returns true if transmit successful
bool RFM_Wrapper::send(String msg)
{
    if (!action_done && !_last_action_transmit)
    {
        // for some reason if it doesn't timeout its sad mybe later ad timout
        _lora.standby();
        action_done = true;
    }

    if (action_done == false)
    {
        return false;
    }
    else
    {
        // reset flag
        action_done = false;
    }

    if (!_last_action_transmit)
    {
        String str;
        int _action_state = _lora.readData(str);
        if (_action_state == RADIOLIB_ERR_NONE)
        {
            _receive_left_over = str;
        }
        _action_state == RADIOLIB_ERR_NONE;
    }
    else
    {
        Serial.println("Transmit finished");
        _action_state == RADIOLIB_ERR_NONE;
    }
    
    // start transmit
    _lora.finishTransmit();
    _action_state = _lora.startTransmit(msg);
    _last_action_transmit = true;
    if (_action_state != RADIOLIB_ERR_NONE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Listen to messages over LoRa. Returns true if received successfully
bool RFM_Wrapper::receive(String &msg, float &rssi, float &snr)
{
    while (!action_done && _last_action_transmit)
    {
        delay(1);
    }

    // Send back anything that was left over when receive was called and then someone called transmit
    if (_receive_left_over != "")
    {
        msg = _receive_left_over;
        _receive_left_over = "";
        return true;
    }

    if (action_done == false)
    {
        return false;
    }
    else
    {
        // reset flag
        action_done = false;
    }

    if (_last_action_transmit)
    {
        Serial.println("Transmit finished");
        _action_state == RADIOLIB_ERR_NONE;
    }
    else
    {
        // Get received data
        String str;
        int _action_state = _lora.readData(str);
        if (_action_state == RADIOLIB_ERR_NONE)
        {
            msg = str; /// maybe later add rssi
            rssi = _lora.getRSSI();
            snr = _lora.getSNR();
        }
        // reset state
        _action_state = RADIOLIB_ERR_NONE;
    }
    // start receive
    _lora.finishTransmit();
    _lora.startReceive();
    _last_action_transmit = false;

    if (msg != "")
    {
        return true;
    }
    else
    {
        return false;
    }
}