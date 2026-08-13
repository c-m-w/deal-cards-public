/// pid_controller.h

#include <Arduino.h>
#include <math.h>

namespace m
{
    class tune_pid;
}

class pid_controller {
private:

    struct _planner_output {
        float _eta;
        unsigned long _elapsed_ms;
        float _x, _xdot, _xddot;
    };

    // Current path variables
    bool _executing = false;
    float _x_start = 0.f;
    float _x_last = 0.f;
    unsigned long _t_last = 0.f;
    float _x_des = 0.f;
    float _integral = 0.f;
    float _xdot_filtered = 0.f;
    unsigned long _start_time_ms = 0.f;

    _planner_output plan_quintic();

protected:

    // Controller variables
    float _K_p;
    float _K_i;
    float _K_d;
    float _K_ff;
    float _angular_speed;
    float _T_ms_min;
    float _T_ms = 0.f;

public:

    pid_controller(float K_p, float K_i, float K_d, float K_ff, float T_ms_min, float angular_speed);
    ~pid_controller() = default;

    void set_goal(float x_start, float x_des);
    float do_pid(float x_current);
    bool executing();

    friend class m::tune_pid;
};