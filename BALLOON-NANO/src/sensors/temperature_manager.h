#include "ClosedCube_STS35.h"
#include "ClosedCube_I2C.h"

class Temperature_Manager
{
    // Heating
    int _heater_pin;
    int _desired_temp;
    const int _pwm_min = 0;
    const int _pwm_max = 255;

    // PID coefficients
    const float _Kp = 15;       // Proportional (Best between 12 and 20)
    const float _Ki = 0.000095; // Integral (Best between 0.00008 and 0.00013)
    const float _Kd = 16000;    // Derivative (Best between 5000 and 20000)

    // PID values
    float _proportional_term = 0;
    float _last_proportional_term = 0;
    float _derivative_term = 0;
    float _integral_term = 0;

    // PID value limits
    const int _proportional_limit = 255;
    const int _integral_limit = 255;
    const int _derivative_limit = 100;

    // Special PID constants
    const float _allowed_overshoot = 0.05;
    const float _integral_penalty = 5;
    
    // Timing
    unsigned long int _last_pid_calculation_time = 0;

    float _inner_temp = 0;
    float _heater_power = 0;

    void set_heater_power();

public:
    void calculate_heater_power(float inner_temp);
    void init(int heater_pin, int desired_temp);
};