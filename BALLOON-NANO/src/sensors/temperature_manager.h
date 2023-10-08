#include "ClosedCube_STS35.h"
#include "ClosedCube_I2C.h"

class Temperature_Manager
{
    float _p, _i, _d;

    int _heater_pin;

    float _inner_t = 0;
    float _outter_t = 0;
    float _heater_power = 0;

    void set_heater_power();

public:
    void calculate_heater_power(float inner_temp, float outter_temp);
    void init(int heater_pin, float P, float I, float D);
};