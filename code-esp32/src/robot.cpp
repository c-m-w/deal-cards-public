/// robot.cpp

#include "main.hpp"

robot::robot() :
    _actrl(
        PCNT_UNIT_0,
        PIN_AXIS_MOTOR_POS,
        PIN_AXIS_MOTOR_NEG,
        0,
        1,
        PIN_ENCODER_POS,
        PIN_ENCODER_NEG,
        PIN_LIM_POS,
        PIN_LIM_NEG
    ),
    _feeder(
        PIN_FEEDER_FORWARD,
        PIN_FEEDER_BACKWARD
    ),
    _launcher(
        2,
        PIN_LAUNCH
    ),
    _vcnt(
        PIN_SEND,
        PIN_RECV
    ),
    _pid(
        0.00150f, // K_p
        0.00100f,    // K_i
        0.00150f, // K_d
        0.00150f,    // K_ff
        400.f,  // T_ms_min
        180.f   // Angular speed
    )
{ 
    _actrl.stop();
    _feeder.stop();
    _launcher.stop();
}

bool robot::do_direction_test() {
    auto const t = millis();

    _actrl.zero_encoder();

    _actrl.move_positive(0.1);

    while (!_actrl.lim_hit() && (millis() - t) < 50ul) {
        delay(1);
    }

    _actrl.stop();

    auto const delta_theta = _actrl.angle();

    Serial.printf("robot::do_homing | Direction test delta theta: %.2f\n", delta_theta);

    if (delta_theta == 0.f) {
        Serial.println("robot::do_direction_test | Expected motion but did not move!");
        return false;
    } else if (delta_theta < 0.f) {
        Serial.println("robot::do_direction_test | Expected to move positive, but moved negative!");
        return false;
    }

    return true;
}

bool robot::do_homing()
{
    if (!do_direction_test()) {
        return false;
    }

    while (!_actrl.neg_lim_hit() && !_actrl.pos_lim_hit()) {
        _actrl.move_negative(0.15f);
        delay(1);
    }

    if (_actrl.pos_lim_hit()) {
        Serial.println("robot::do_homing | Expected to hit negative limit, but hit positive!");
        return false;
    }

    _actrl.zero_encoder();

    bool neg_lim_released = false;

    while (!_actrl.pos_lim_hit() && (!_actrl.neg_lim_hit() || !neg_lim_released)) {
        const float duty = _actrl.angle() < 300.f ? 0.8f : 0.15f;
        _actrl.move_positive(duty);
        delay(1);

        if (!_actrl.neg_lim_hit()) {
            neg_lim_released = true;
        }
    }

    if (_actrl.neg_lim_hit()) {
        Serial.println("robot::do_homing | Expected to hit positive limit, but hit negative!");
        return false;
    }

    _actrl.set_limits(axis_control::_LIMIT_MARGIN, _actrl.angle() - axis_control::_LIMIT_MARGIN);

    while (_actrl.angle() > 180.f) {
        _actrl.move_negative(1.f);
        delay(1);
    }

    _actrl.stop();

    return true;
}

void robot::move(float speed)
{
    auto const lims = _actrl.get_limits();
    speed = std::clamp(speed, -1.f, 1.f);

    if (std::abs(speed) < 0.05f)
    {
        _actrl.stop();
    }
    else if (speed > 0.f)
    {
        if (_actrl.close_to_pos_lim())
        {
            speed = std::min(0.1f, speed);
        }

        _actrl.move_positive(speed);
    }
    else if (speed < 0.f)
    {
        speed = -speed;

        if (_actrl.close_to_neg_lim())
        {
            speed = std::min(0.1f, speed);
        }

        _actrl.move_negative(speed);
    }
}

void robot::move_to(float angle)
{
    const auto limits = _actrl.get_limits();

    if (angle < limits[0] || angle > limits[1])
    {
        Serial.printf("robot::move_to | Angle out of bounds: %.2f\n", angle);
        return;
    }

    _actrl.stop();

    Serial.println(">clear");

    _pid.set_goal(_actrl.angle(), angle);

    while (_pid.executing())
    {
        auto const spd = _pid.do_pid(_actrl.angle());
        move(spd);
        delay(1);
    }

    _actrl.stop();

    auto const angle_cur = _actrl.angle();
    auto const err = angle - angle_cur;
    Serial.printf("robot::move_to | Finished PID. Current: %.2f, goal: %.2f. Error: %.2f\n", angle_cur, angle, err);
}

void robot::fire()
{
    _actrl.stop();
	_launcher.set_speed(0.25f);
	delay(300);
	_feeder.feed_forward();

	while (!_vcnt.poll())
	{
		delay(1);
	}

	Serial.println("robot::launch_card | Detected card launch! Stopping...");
	_feeder.stop();
	_launcher.stop();
}
