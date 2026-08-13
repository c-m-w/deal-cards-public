/// vcnt2030.cpp

#include "../main.hpp"

vcnt2030::vcnt2030(int pin_send, int pin_recv) :
    _pin_send(pin_send), _pin_recv(pin_recv)
{
    pinMode(_pin_send, OUTPUT);
    pinMode(_pin_recv, INPUT);
}

bool vcnt2030::poll()
{
    digitalWrite(_pin_send, HIGH);
    delayMicroseconds(_DELAY_MICROSECONDS);
    const int val = analogRead(_pin_recv);
    digitalWrite(_pin_send, LOW);

    return val >= _DETECT_THRESHOLD;
}
