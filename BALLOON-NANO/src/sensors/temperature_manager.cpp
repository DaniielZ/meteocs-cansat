#include "temperature_manager.h"

void Temperature_Manager::calculate_heater_power(float inner_temp)
{
    // Time since last pid update
    float dt = (millis() - _last_pid_calculation_time);

    // Proportional term
    _proportional_term = _desired_temp - inner_temp;

    // Integral term
    // NOT SURE IF REALLY REQUIRED, MORE TESTING WITH MORE EXTREME TEMPERATURES IS NEEDED
    // If the temperature is starting to go over the desired temperature,
    // add a penalty to integral term calculation to try and quickly eliminate temperature overshoot
    if (_proportional_term >= -_allowed_overshoot)
    {
        _integral_term += _proportional_term * dt;
    }
    else
    {
        _integral_term += _integral_penalty * _proportional_term * dt;
    }

    // Derivative term
    _derivative_term = (_proportional_term - _last_proportional_term) / dt;

    // Heater power is the sum of all the individual values multiplied by their coefficients
    // The values need to be constrained to not become too extreme
    _heater_power = constrain(_Kp * _proportional_term, -_proportional_limit, _proportional_limit);
    _heater_power += constrain(_Ki * _integral_term, -_integral_limit, _integral_limit);
    _heater_power += constrain(_Kd * _derivative_term, -_derivative_limit, 0);

    // Constrain the final sum to PWM output range
    _heater_power = constrain(_heater_power, _pwm_min, _pwm_max);

    // Save last proportional term for future derivative term calculations
    _last_proportional_term = _proportional_term;

    // Save last PID calculation time for time delta calculations
    _last_pid_calculation_time = millis();
}
double Temperature_Manager::get_heater_power()
{
    return _heater_power;
}
void Temperature_Manager::set_heater_power()
{
    // Set heater PWM value
    analogWrite(_heater_pin, _heater_power);
}

void Temperature_Manager::init(int heater_pin, int desired_temp)
{
    _heater_pin = heater_pin;
    _desired_temp = desired_temp;
}