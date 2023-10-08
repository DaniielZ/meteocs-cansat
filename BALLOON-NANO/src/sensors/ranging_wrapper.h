#include <RadioLib.h>
#include <Array.h>

class Ranging_Wrapper
{
    // RANGING LORA SPI1
public:
    enum Mode
    {
        SLAVE,
        MASTER
    };
    struct Positon
    {
        float lat = 0;
        float lng = 0;
        float height = 0;
    };
    struct Ranging_Slave
    {
        Positon position;
        long address;
    };
    struct Lora_Device
    {
        float FREQUENCY;
        int CS;
        int DIO0; // busy pin
        int DIO1;
        int RESET;
        int SYNC_WORD;

        int TXPOWER;
        int SPREADING;
        int CODING_RATE;
        float SIGNAL_BW;
        SPIClassRP2040 *SPI;
    };
    struct Ranging_Result
    {
        float distance = 0;
        int time = 0;
    };

private:
    SX1280 _lora = new Module(-1, -1, -1);
    bool _lora_initialized = false;

    unsigned long _ranging_start_time = 0;
    int _lora_range_state;

    Mode _mode;
    Lora_Device _config;

public:
    String init(Mode mode, Lora_Device config);
    bool master_read(Ranging_Slave slave, Ranging_Result &result, long int timeout);
    void slave_reenable();
    bool trilaterate_position(Ranging_Result readings[3], Ranging_Slave slaves[3], Positon &result);
};