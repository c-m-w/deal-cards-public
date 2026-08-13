/// axis_control.cpp

#include "main.hpp"

void axis_control::setup_pwm()
{
    ledcSetup(_pwm_ch_pos, _DRIVER_MOTOR_FREQ, _DRIVER_MOTOR_RES);
    ledcAttachPin(_ppwm_pos, _pwm_ch_pos);
    
    ledcSetup(_pwm_ch_neg, _DRIVER_MOTOR_FREQ, _DRIVER_MOTOR_RES);
    ledcAttachPin(_ppwm_neg, _pwm_ch_neg);

    stop();
}

void axis_control::setup_limit_switches()
{
    pinMode(_plim_pos, INPUT_PULLUP);
    pinMode(_plim_neg, INPUT_PULLUP);
}

void axis_control::setup_encoder() {
    pcnt_config_t pcnt_A = {
        .pulse_gpio_num = _penc_pos,
        .ctrl_gpio_num  = _penc_neg,
        .lctrl_mode = PCNT_CHANNEL_LEVEL_ACTION_INVERSE,
        .hctrl_mode = PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        .pos_mode       = PCNT_COUNT_INC,    // Count up when B is LOW
        .neg_mode       = PCNT_COUNT_DEC,    // Ignore falling edge of A
        .counter_h_lim  = 32767,
        .counter_l_lim  = -32768,
        .unit           = _pcnt_unit,
        .channel        = PCNT_CHANNEL_0
    };
    pcnt_config_t pcnt_B = {
        .pulse_gpio_num = _penc_neg,
        .ctrl_gpio_num  = _penc_pos,
        .lctrl_mode = PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        .hctrl_mode = PCNT_CHANNEL_LEVEL_ACTION_INVERSE,
        .pos_mode       = PCNT_COUNT_INC,    // Count up when B is LOW
        .neg_mode       = PCNT_COUNT_DEC,    // Ignore falling edge of A
        .counter_h_lim  = 32767,
        .counter_l_lim  = -32768,
        .unit           = _pcnt_unit,
        .channel        = PCNT_CHANNEL_1
    };

    pcnt_unit_config(&pcnt_A);
    pcnt_unit_config(&pcnt_B);
    pcnt_set_filter_value(PCNT_UNIT_0, 100);
    pcnt_filter_enable(PCNT_UNIT_0);

    zero_encoder();
}

axis_control::axis_control(
    pcnt_unit_t pcnt_unit,
    int ppwm_pos,
    int ppwm_neg,
    int pwm_channel_pos,
    int pwm_channel_neg,
    int penc_pos,
    int penc_neg,
    int plim_pos,
    int plim_neg) :
    _pcnt_unit(pcnt_unit),
    _ppwm_pos(ppwm_pos),
    _ppwm_neg(ppwm_neg),
    _pwm_ch_pos(pwm_channel_pos),
    _pwm_ch_neg(pwm_channel_neg),
    _penc_pos(penc_pos),
    _penc_neg(penc_neg),
    _plim_pos(plim_pos),
    _plim_neg(plim_neg)
{
    setup_pwm();
    setup_limit_switches();
    setup_encoder();
}

void axis_control::move_positive(float duty)
{
    if (pos_lim_hit()) {
        stop();
        return;
    }

    const uint8_t integer_duty = (1.f - duty) * _MAX_DUTY;

    ledcWrite(_pwm_ch_pos, _MAX_DUTY);
    ledcWrite(_pwm_ch_neg, integer_duty);
    _current_duty = duty;
}

void axis_control::move_negative(float duty)
{
    if (neg_lim_hit()) {
        stop();
        return;
    }

    const uint8_t integer_duty = (1.f - duty) * _MAX_DUTY;

    ledcWrite(_pwm_ch_neg, _MAX_DUTY);
    ledcWrite(_pwm_ch_pos, integer_duty);
    _current_duty = -duty;
}

void axis_control::stop()
{
    ledcWrite(_pwm_ch_pos, _MAX_DUTY);
    ledcWrite(_pwm_ch_neg, _MAX_DUTY);
    _current_duty = 0.f;
}

void axis_control::stop_if_lim_hit() {
    if (_current_duty == 0.f) {
        return;
    }

    if (_current_duty > 0.f && pos_lim_hit()) {
        stop();
    }

    if (_current_duty < 0.f && neg_lim_hit()) {
        stop();
    }
}

void axis_control::set_limits(float neg, float pos)
{
    if (neg > pos) {
        Serial.println("axis_control::set_limits | Error: neg > pos");
    }

    _neg_lim = neg;
    _pos_lim = pos;

    Serial.printf("axis_control::set_limits | New limits: [%.2f, %.2f]", neg, pos);
}

bool axis_control::close_to_pos_lim()
{
    return std::abs(angle() - _pos_lim) < _CLOSE_MARGIN;
}

bool axis_control::close_to_neg_lim()
{
    return std::abs(angle() - _neg_lim) < _CLOSE_MARGIN;
}

bool axis_control::pos_lim_hit()
{
    return digitalRead(_plim_pos) == LOW;
}

bool axis_control::neg_lim_hit()
{
    return digitalRead(_plim_neg) == LOW;
}

bool axis_control::lim_hit()
{
    return pos_lim_hit() || neg_lim_hit();
}

float axis_control::angle()
{
    int16_t count = 0;
    const float gearRatio = 1225.f / 12.f;

    pcnt_get_counter_value(_pcnt_unit, &count);

    const float angle = (float)count / 64.0 / gearRatio * 360.0;

    return angle;
} 

void axis_control::print_angle()
{
    Serial.printf("axis_control::print_angle | Angle: %.2f\n", angle());
}

void axis_control::zero_encoder()
{
    pcnt_counter_pause(_pcnt_unit);
    pcnt_counter_clear(_pcnt_unit);
    pcnt_counter_resume(_pcnt_unit);
}

std::array<float, 2> axis_control::get_limits()
{
    return std::array<float, 2> { _neg_lim, _pos_lim };
}
