#include "ranging_wrapper.h"
#include <math.h>

volatile bool sx1280_lora_ranging = false; // can't be moved inside class. Shows if lora is actively doing ranging
void sx1280_ranging_end(void)
{
    sx1280_lora_ranging = false;
}

double Ranging_Wrapper::Position_Local::length()
{
    return sqrt(pow(this->x, 2) + pow(this->x, 2) + pow(this->x, 2));
}

Ranging_Wrapper::Position_Local Ranging_Wrapper::Position_Local::operator-(Position_Local &other)
{
    Position_Local result;
    result.x = this->x - other.x;
    result.y = this->y - other.y;
    result.z = this->z - other.z;
    return result;
}
Ranging_Wrapper::Position_Local Ranging_Wrapper::Position_Local::operator+(Position_Local &other)
{
    Position_Local result;
    result.x = this->x + other.x;
    result.y = this->y + other.y;
    result.z = this->z + other.z;
    return result;
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

void Ranging_Wrapper::local_point_to_global_space(Position_Local movable_point, Position p[3], Position &result)
{
    // calculate plane angles

    // make transformation matrix

    // apply transformation matrix
    result;
}
// the x-axis goes through long,lat (0,0), so longitude 0 meets the equator;
// the y - axis goes through(0, 90);
// and the z - axis goes through the poles
Ranging_Wrapper::Position_Local Ranging_Wrapper::Position::to_absolute_cartesian()
{
    Position_Local result;
    double lat = this->lat * DEG_TO_RAD;
    double lng = this->lng * DEG_TO_RAD;

    result.x = _earth_radius * cos(lat) * sin(lng);
    result.y = _earth_radius * cos(lat) * sin(lng);
    result.z = (_earth_radius + this->height) * sin(lat);

    return result;
}

double Ranging_Wrapper::distance_between_earth_cordinates_m(Position p1, Position p2)
{
    // Position d_pos = {};
    // d_pos.lat = DEG_TO_RAD * (p2.lat - p1.lat);
    // d_pos.lng = DEG_TO_RAD * (p2.lng - p1.lng);
    // d_pos.height = p2.height - p1.height;

    // p1.lat = DEG_TO_RAD * p1.lat;
    // p2.lat = DEG_TO_RAD * p2.lat;

    // double a = sin(d_pos.lat / 2.0) * sin(d_pos.lat / 2.0) + sin(d_pos.lng / 2.0) * sin(d_pos.lng / 2.0) * cos(p1.lat) * cos(p2.lat);
    // double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    // double xy_distance = c * _earth_radius;

    // double xyz_distance = sqrt(pow(xy_distance, 2) + pow(d_pos.height, 2));

    return;
}

bool Ranging_Wrapper::trilaterate_position(Ranging_Result readings[3], Ranging_Slave slaves[3], Position &result)
{
    /// set global space
    Position global_bs_pos[3];

    for (int i = 0; i < 3; i++)
    {
        global_bs_pos[i] = slaves[i].position;
    }

    /// global space to local space
    Position_Local local_bs_pos[3];
    // lets set the 0, 0, 0 as the first index and base everything of that
    local_bs_pos[0] = {0, 0, 0};
    double distance1_2 = distance_between_earth_cordinates_m(slaves[0].position, slaves[1].position);
    double distance1_3 = distance_between_earth_cordinates_m(slaves[0].position, slaves[2].position);
    double distance2_3 = distance_between_earth_cordinates_m(slaves[1].position, slaves[2].position);
    local_bs_pos[1] = {distance1_2, 0, 0};
    // calculate the third point
    local_bs_pos[2].y = (pow(distance1_2, 2) + pow(distance1_3, 2) - pow(distance2_3, 2)) / (2 * distance1_2); // add check if its trying to devide by 0
    local_bs_pos[2].x = sqrt(pow(distance1_3, 2) - pow(local_bs_pos[2].y, 2));
    local_bs_pos[2].z = 0;

    /// TODO check if data is usable if not return false;
    // is reading too small?, is reading too large?

    /// calculate pos in local space    https://en.wikipedia.org/wiki/True-range_multilateration#Three_Cartesian_dimensions,_three_measured_slant_ranges
    Position_Local calculated_local_pos;
    calculated_local_pos.x = (pow(readings[0].distance, 2) - pow(readings[1].distance, 2)) / (2 * local_bs_pos[2].x);
    calculated_local_pos.y = (pow(readings[0].distance, 2) - pow(readings[2].distance, 2) + pow(local_bs_pos[2].x, 2), pow(local_bs_pos[2].y, 2) - (2 * local_bs_pos[2].x * calculated_local_pos.x)) / (2 * local_bs_pos[2].y);
    calculated_local_pos.z = sqrt(pow(readings[1].distance, 2) - pow(calculated_local_pos.x, 2) - pow(calculated_local_pos.y, 2)); // this can be either positive or negative

    /// local space to global space
    Position calculated_global_pos;
    // calculate transofrmation matrix to do that first you need to calculate local space angles relative to global
    // x
    // y
    // z

    /// set results
    result = calculated_global_pos;
    return true;
}
