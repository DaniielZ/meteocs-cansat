#include "ClosedCube_STS35.h"
#include "ClosedCube_I2C.h"

class Temperature_Manager
{
    // Heating
    int _heater_pin;
    float _desired_temp; // in C
    float _safe_temp;
    const int _pwm_min = 0;
    // This should limit the max heater power to around 3 watts
    const int _pwm_max = 32;

    // PID coefficients
    const float _Kp = 7.5;     // Proportional (Best between 5 and 15)
    const float _Ki = 0.00001; // Integral (Best between 0.00008 and 0.00013)
    const float _Kd = 10000;   // Derivative (Best between 5000 and 20000)

    // PID values
    float _proportional_term = 0;
    float _last_proportional_term = 0;
    float _derivative_term = 0;
    float _integral_term = 0;

    // PID value limits
    const int _proportional_limit = _pwm_max;
    const int _integral_limit = _pwm_max;
    const int _derivative_limit = _pwm_max / 2;

    // Timing
    unsigned long int _last_pid_calculation_time = 0;
    unsigned long int _time_at_safe_temp_start = 0;

    float _inner_temp = 0;
    float _heater_power = 0;

public:
    void set_heater_power();
    void calculate_heater_power(float inner_temp);
    double get_heater_power();
    void reset() { _last_pid_calculation_time = millis(); };
    void get_pid(float &p, float &i, float &d);
    void init(int heater_pin, int desired_temp);
};