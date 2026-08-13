/// vcnt2030.hpp

class vcnt2030
{
private:

    const int _DELAY_MICROSECONDS = 100;
    const int _DETECT_THRESHOLD = 4000;
    int _pin_send, _pin_recv;

public:

    vcnt2030(int pin_send, int pin_recv);
    ~vcnt2030() = default;

    bool poll();
};
