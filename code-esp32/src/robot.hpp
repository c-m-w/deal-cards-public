/// robot.hpp

class robot
{
private:
    // Stage motion
    const int PIN_LIM_POS = 42;
    const int PIN_LIM_NEG = 41;
    const int PIN_AXIS_MOTOR_POS = 19;
    const int PIN_AXIS_MOTOR_NEG = 20;
    const int PIN_ENCODER_POS = 2;
    const int PIN_ENCODER_NEG = 1;

    // Feeder motor
    const int PIN_FEEDER_FORWARD = 21;
    const int PIN_FEEDER_BACKWARD = 47;

    // Launch motor
    const int PIN_LAUNCH = 48;

    // VCNT2030
    const int PIN_SEND = 4;
    const int PIN_RECV = 5;

public:

    axis_control _actrl;
    feeder_motor _feeder;
    launch_motor _launcher;
    vcnt2030 _vcnt;
    pid_controller _pid;

    bool do_direction_test();

public:

    robot();
    ~robot() = default;

    bool do_homing();
    void move(float speed);
    void move_to(float angle);
    void fire();

    friend class tune_pid;
};
