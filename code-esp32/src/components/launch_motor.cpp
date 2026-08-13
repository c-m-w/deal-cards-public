/// launch_motor.cpp

#include "../main.hpp"

launch_motor::launch_motor(int channel, int pin) :
    _channel(channel), _pin(pin)
{
    ledcSetup(_channel, _FREQ, _RES);
    ledcAttachPin(_pin, _channel);

    set_speed(0.f);
}

void launch_motor::set_speed(float speed)
{
    speed = std::clamp(speed, 0.f, 1.f);

    const float pulse_width = 1000.f + 1000.f * speed;
    const float period = 1e6 / float(_FREQ);
    const int duty = (pulse_width / period) * float(_MAX_DUTY);

    ledcWrite(_channel, duty);
}

void launch_motor::stop()
{
    set_speed(0.f);
}
