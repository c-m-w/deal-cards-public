/// main.hpp

constexpr int DISPLAY_WIDTH = 240;
constexpr int DISPLAY_HEIGHT = 320;

#include <Arduino.h>
#include <driver/pcnt.h>
#include <LovyanGFX.hpp>
#include <AS5600.h>
#include <Wire.h>
#include <deque>
#include <stack>
#include <mutex>
#include <atomic>

#include "helpers.hpp"

#include "components/button.hpp"
#include "components/feeder_motor.hpp"
#include "components/launch_motor.hpp"
#include "components/vcnt2030.hpp"
#include "components/display/lgfx_device.hpp"

#include "axis_control.hpp"
#include "pid_controller.h"
#include "robot.hpp"

#include "menu/menu.hpp"
#include "menu/screens/homing_prompt.hpp"
#include "menu/screens/homing.hpp"
#include "menu/screens/tune_pid.hpp"

#include "components/display/display.hpp"

#include "controller.hpp"

inline robot rob;
inline controller ctl;
