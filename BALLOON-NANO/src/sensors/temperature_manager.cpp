#include "temperature_manager.h"

void Temperature_Manager::calculate_heater_power(float inner_temp, float outter_temp)
{
    // TODO implementation
    // called many times per sec;
}
void Temperature_Manager::set_heater_power()
{
    // TODO implementation
    _heater_power;
    _heater_pin;
}
void Temperature_Manager::init(int heater_pin, float p, float i, float d)
{
    _heater_pin = heater_pin;
    _p = p;
    _i = i;
    _d = d;
}