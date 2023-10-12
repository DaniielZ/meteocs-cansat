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

    struct Position_Local
    {
        double x = 0;
        double y = 0;
        double z = 0;
        double length();
        Position_Local operator-(Position_Local &other);
        Position_Local operator+(Position_Local &other);
    };

    struct Position
    {
        double lat = 0;
        double lng = 0;
        double height = 0;
        Position_Local to_absolute_cartesian();

    private:
        double _earth_radius = 6378000; // m
    };
    struct Ranging_Slave
    {
        Position position; // in degrees
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
        double distance = 0;
        int time = 0;
    };

private:
    SX1280 _lora = new Module(-1, -1, -1);
    bool _lora_initialized = false;

    unsigned long _ranging_start_time = 0;
    int _lora_range_state;

    Mode _mode;
    Lora_Device _config;
    double distance_between_earth_cordinates_m(Position p1, Position p2);

    void local_point_to_global_space(Position_Local movable_point, Position p[3], Position &result);

public:
    String init(Mode mode, Lora_Device config);
    bool master_read(Ranging_Slave slave, Ranging_Result &result, long int timeout);
    void slave_reenable();
    bool trilaterate_position(Ranging_Result readings[3], Ranging_Slave slaves[3], Position &result);
};