/// button.cpp

#include "../main.hpp"

button::button(int pin): _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);
}

bool button::pressed()
{
    return digitalRead(_pin) == LOW;
}

button::operator bool()
{
    return pressed();
}
