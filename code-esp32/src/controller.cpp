/// controller.cpp

#include "main.hpp"

controller::controller() :
    _btn_enter(PIN_BTN_ENTER), _btn_up(PIN_BTN_UP), _btn_dn(PIN_BTN_DN),
    _disp(PIN_DISP_CLK, PIN_DISP_D, PIN_DISP_RST, PIN_DISP_DC, PIN_DISP_CS)
{ }

bool controller::init()
{
    if (!_disp.init())
    {
        return false;
    }

    if (!Wire.begin(PIN_RTENC_D, PIN_RTENC_CLK))
    {
        Serial.println("controller::init | Failed to begin i2c channel!");
        return false;
    }

    if (!_rtenc.begin())
    {
        Serial.println("controller::init | Failed to begin Rotary encoder (AS5600) on i2c!");
        return false;
    }

    if (!_rtenc.magnetDetected())
    {
        Serial.println("controller::init | Rotary encoder (AS5600) magnet not detected!");
        return false;
    }

    if (_rtenc.magnetTooStrong())
    {
        Serial.println("controller::init | Rotary encoder (AS5600) magnet too strong!");
        return false;
    }

    if (_rtenc.magnetTooWeak())
    {
        Serial.println("controller::init | Rotary encoder (AS5600) magnet too weak!");
        return false;
    }

    Serial.print("controller::init | Rotary encoder (AS5600) ready on: 0x");
    Serial.println(_rtenc.getAddress(), HEX);

    return true;
}

float controller::get_ctl_angle()
{
    int32_t raw = _rtenc.getCumulativePosition();
    float angle = raw * AS5600_RAW_TO_DEGREES; // 360.0 / 4096.0

    return fmod(angle + 360.f, 360.f);
}

float controller::get_ctl_speed()
{
    static std::deque<float> recent_speeds;
    static float sum = 0.0f;
    constexpr size_t WINDOW_SIZE = 20;

    float new_speed = _rtenc.getAngularSpeed(AS5600_MODE_DEGREES) / 360.0f;

    if (!std::isnan(new_speed) && _rtenc.magnetDetected() && !_rtenc.magnetTooStrong() && !_rtenc.magnetTooWeak())
    {
        recent_speeds.push_back(new_speed);
        sum += new_speed;

        if (recent_speeds.size() > WINDOW_SIZE)
        {
            sum -= recent_speeds.front();
            recent_speeds.pop_front();
        }
    }

    if (recent_speeds.empty())
        return 0.0f;

    const auto avg = sum / recent_speeds.size();

    if (std::abs(avg) < 0.01f)
        return 0.f;

    return avg;
}

void controller::set_angle(float target)
{
    _rtenc.resetCumulativePosition();
    _rtenc.setOffset(0.f);
    float current = get_ctl_angle();
    float offset = target - current;

    if (offset < 0.f) {
        offset += 360.f;
    }

    _rtenc.setOffset(offset);
}

void controller::clamp_angle(std::array<float, 2> limits)
{
    if (limits[1] < limits[0])
    {
        Serial.println("controller::clamp_angle | Error: limits[1] < limits[0]!");
    }

    auto const ctl_angle = get_ctl_angle();

    if (ctl_angle < limits[0])
    {
        set_angle(limits[0]);
        auto const n = get_ctl_angle();
        Serial.printf("controller::clamp_angle | Control angle clamped to %.2f. New angle: %.2f\n", limits[0], n);
    }
    else if (ctl_angle > limits[1])
    {
        set_angle(limits[1]);
        auto const n = get_ctl_angle();
        Serial.printf("controller::clamp_angle | Control angle clamped to %.2f. New angle: %.2f\n", limits[1], n);

    }
}
