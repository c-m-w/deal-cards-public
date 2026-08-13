/// helpers.cpp

#include "main.hpp"

namespace helpers
{
    std::string float_to_string(float f, int decimals)
    {
        static char buf[16];
        dtostrf(f, 0, decimals, buf);
        return std::string(buf);
    }
}

