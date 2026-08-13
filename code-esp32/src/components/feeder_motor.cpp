/// feeder_motor.cpp

#include "../main.hpp"

feeder_motor::feeder_motor(int pin_forward, int pin_backward) :
    _pin_forward(pin_forward), _pin_backward(pin_backward)
{ 
    pinMode(_pin_forward, OUTPUT);
    pinMode(_pin_backward, OUTPUT);

    stop();
}

void feeder_motor::feed_forward()
{
    digitalWrite(_pin_forward, HIGH);
    digitalWrite(_pin_backward, LOW);
}

void feeder_motor::feed_backward()
{
    digitalWrite(_pin_forward, LOW);
    digitalWrite(_pin_backward, HIGH);
}

void feeder_motor::stop()
{
    digitalWrite(_pin_forward, LOW);
    digitalWrite(_pin_backward, LOW);
}