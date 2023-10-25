#include "temperature_manager.h"

void Temperature_Manager::calculate_heater_power(float inner_temp)
{
    // Time since last pid update
    float dt = (millis() - _last_pid_calculation_time);
    // If the temperature is 0.1 degree below or above the safe temp, reset the timer
    if ((_safe_temp - 0.1) > inner_temp || inner_temp > (_safe_temp + 0.1))
    {
        _time_at_safe_temp_start = millis();
    }

    // Calculate proportional term based on the state
    if (_safe_temp == _desired_temp)
    {
        _proportional_term = _desired_temp - inner_temp;
    }
    else
    {
        // If the temperature has stayed at the safe temp for 15 seconds, increase the safe temperature
        if ((millis() - _time_at_safe_temp_start) > 15000)
        {
            _safe_temp += 1;
            _time_at_safe_temp_start = millis();
        }
        _proportional_term = _safe_temp - inner_temp;
    }

    // Integral term
    if (_integral_term < 0)
    {
        _integral_term = 0;
    }
    else
    {
        _integral_term += _proportional_term * dt;
    }

    // Derivative term
    _derivative_term = (_proportional_term - _last_proportional_term) / dt;

    // Heater power is the sum of all the individual values multiplied by their coefficients
    // The values need to be constrained to not become too extreme
    Serial.println();
    Serial.print("Safe temp: " + String(_safe_temp, 5));
    _heater_power = constrain(_Kp * _proportional_term, -_proportional_limit, _proportional_limit);
    Serial.print("P: " + String(_Kp * _proportional_term));
    _heater_power += constrain(_Ki * _integral_term, 0, _integral_limit);
    Serial.print("  I: " + String(_Ki * _integral_term, 5));
    _heater_power += constrain(_Kd * _derivative_term, -_derivative_limit, 0);
    Serial.println("  D: " + String(_Kd * _derivative_term, 5));

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
    pinMode(_heater_pin, OUTPUT_12MA);
    analogWrite(_heater_pin, _heater_power);
}
void Temperature_Manager::get_pid(float &p, float &i, float &d)
{
    p = _proportional_term * _Kp;
    i = _integral_term * _Ki;
    d = _derivative_term * _Kd;
}

void Temperature_Manager::init(int heater_pin, int desired_temp)
{
    _heater_pin = heater_pin;
    _desired_temp = desired_temp;
    _safe_temp = _desired_temp - 5;
}