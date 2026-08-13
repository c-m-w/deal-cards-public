/// launch_motor.hpp

class launch_motor
{
private:

    const int _FREQ = 50;
    const int _RES = 14;
    const int _MAX_DUTY = (1 << _RES) - 1;

    int _channel, _pin;

public:

    launch_motor(int channel, int pin);
    ~launch_motor() = default;

    void set_speed(float speed);
    void stop();
};
