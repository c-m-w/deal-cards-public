/// feeder_motor.hpp

class feeder_motor
{
private:

    int _pin_forward, _pin_backward;

public:

    feeder_motor(int pin_forward, int pin_backward);
    ~feeder_motor() = default;

    void feed_forward();
    void feed_backward();
    void stop();
};
