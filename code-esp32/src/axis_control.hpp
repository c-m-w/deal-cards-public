/// axis_control.hpp

#include <driver/pcnt.h>

class axis_control {
private:

    const int _DRIVER_MOTOR_FREQ = 20000;
    const int _DRIVER_MOTOR_RES = 8;
    const int _MAX_DUTY = __UINT8_MAX__;

    int _ppwm_pos, _ppwm_neg;
    int _pwm_ch_pos, _pwm_ch_neg;
    int _penc_pos, _penc_neg;
    int _plim_pos, _plim_neg;
    pcnt_unit_t _pcnt_unit;
    float _current_duty = 0.f;
    // Coded limits for stage motion
    float _neg_lim = -INFINITY;
    float _pos_lim = INFINITY;

    void setup_pwm();
    void setup_limit_switches();
    void setup_encoder();

public:

    // Margin of safety around limit switches
    static constexpr float _LIMIT_MARGIN = 1.f;
    static constexpr float _CLOSE_MARGIN = 10.f;

    axis_control(
        pcnt_unit_t pcnt_unit,
        int ppwm_pos,
        int ppwm_neg,
        int pwm_channel_pos,
        int pwm_channel_neg,
        int penc_pos,
        int penc_neg,
        int plim_pos,
        int plim_neg);
    ~axis_control() = default;

    void move_positive(float duty);
    void move_negative(float duty);
    void stop();
    void stop_if_lim_hit();
    void set_limits(float neg, float pos);
    bool close_to_pos_lim();
    bool close_to_neg_lim();
    bool pos_lim_hit();
    bool neg_lim_hit();
    bool lim_hit();
    float angle();
    void print_angle();
    void zero_encoder();
    // Returns [negative, positive]
    std::array<float, 2> get_limits();
};
