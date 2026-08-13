/// pid_controller.cpp

#include "pid_controller.h"

pid_controller::pid_controller(float K_p, float K_i, float K_d, float K_ff, float T_ms_min, float _angular_speed)
    : _K_p(K_p), _K_i(K_i), _K_d(K_d), _K_ff(K_ff), _T_ms_min(T_ms_min), _angular_speed(_angular_speed)
{ }

pid_controller::_planner_output pid_controller::plan_quintic() {

    const unsigned long elapsed_ms = millis() - _start_time_ms;
    // Control variable [0, 1]
    const float eta = (float)(elapsed_ms) / _T_ms;

    if (eta >= 1.f) {
        _executing = false;
    }

    const float T_s = _T_ms / 1000.f;

    const float error = _x_des - _x_start;
    const float x = _x_start + error * (10 * pow(eta, 3) - 15 * pow(eta, 4) + 6 * pow(eta, 5));
    const float xdot = error / T_s * (30 * pow(eta, 2) - 60 * pow(eta, 3) + 30 * pow(eta, 4));
    const float xddot = error / pow(T_s, 2) * (60 * eta - 180 * pow(eta, 2) + 120 * pow(eta, 3));

    return _planner_output {
        ._eta = eta,
        ._elapsed_ms = elapsed_ms,
        ._x = x,
        ._xdot = xdot,
        ._xddot = xddot
    };
}

void pid_controller::set_goal(float x_start, float x_des) {
    _executing = true;
    _x_start = x_start;
    _x_last = x_start;
    _t_last = millis();
    _x_des = x_des;
    _integral = 0.f;
    _xdot_filtered = 0.f;
    _start_time_ms = millis();
    _T_ms = std::max(_T_ms_min, abs(x_start - x_des) / _angular_speed * 1000.f);
    Serial.printf("Start PID with period: %.2f\n", _T_ms);
}

float pid_controller::do_pid(float x_current) {

    const auto out = plan_quintic();
    
    if (!_executing) {
        return 0;
    }

    const float e_x = out._x - x_current;

    // Calculate xdot
    const unsigned long t = millis();
    const float delta_t = (float)(t - _t_last) / 1000.f;
    const float delta_x = x_current - _x_last;
    const float alpha = 0.2f;

    if (delta_t < 1e-5)
    {
        _xdot_filtered = 0.f;
    }
    else
    {
        _xdot_filtered = alpha * delta_x / delta_t + (1.f - alpha) * _xdot_filtered;
    }

    const float e_xdot = out._xdot - _xdot_filtered;

    Serial.printf(">x:current(%.4f,%.4f)\n", out._eta, x_current);
    Serial.printf(">x:desired(%.4f,%.4f)\n", out._eta, out._x);
    Serial.printf(">x:error(%.4f,%.2f)\n", out._eta, e_x);

    Serial.printf(">xdot:current(%.4f,%.4f)\n", out._eta, _xdot_filtered);
    Serial.printf(">xdot:desired(%.4f,%.4f)\n", out._eta, out._xdot);
    Serial.printf(">xdot:error(%.4f,%.2f)\n", out._eta, e_xdot);

    _integral += e_x * delta_t;

    const float SPEED_MIN = 0.1f;
    float speed = _K_p * e_x + _K_i * _integral + _K_d * e_xdot + _K_ff * out._xdot;

    if (speed > 0.f)
    {
        speed += SPEED_MIN;
    }
    else
    {
        speed -= SPEED_MIN;
    }

    // Anti-windup: only integrate when not saturated
    if (abs(speed) >= 1.f) {
        _integral -= e_x * delta_t;
    }

    _x_last = x_current;
    _t_last = t;

    return speed;
}

bool pid_controller::executing() {
    return _executing;
}
