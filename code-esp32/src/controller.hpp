/// controller.hpp

class controller {
private:

    // Buttons
    const int PIN_BTN_ENTER = 9;
    const int PIN_BTN_UP = 46;
    const int PIN_BTN_DN = 3;

    // Display
    const int PIN_DISP_CLK = 12;
    const int PIN_DISP_D = 11;
    const int PIN_DISP_RST = 13;
    const int PIN_DISP_DC = 14;
    const int PIN_DISP_CS = 10;

    // Rotary encoder
    const int PIN_RTENC_CLK = 18;
    const int PIN_RTENC_D = 8;

public:

    display _disp;
    button _btn_enter;
    button _btn_up;
    button _btn_dn;
    AS5600 _rtenc;

    controller();
    ~controller() = default;

    bool init();
    float get_ctl_angle();
    float get_ctl_speed();
    void set_angle(float target);
    void clamp_angle(std::array<float, 2> limits);
};
