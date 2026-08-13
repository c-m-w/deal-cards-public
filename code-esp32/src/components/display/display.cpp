/// display.cpp

#include "../../main.hpp"

display::display(int pin_clock, int pin_data, int pin_rst, int pin_dc, int pin_cs) :
    _dev(pin_clock, pin_data, pin_rst, pin_dc, pin_cs),
    _sprite(&_dev)
{ }

bool display::init()
{
    if (!_dev.init())
    {
        const auto error = _dev.getWriteError();
        Serial.printf("display::init | Failed to init device! Error: %d\n", error);
        return false;
    }

    _dev.setRotation(1);
    _dev.setTextSize(3);

    Serial.printf("display::init | Device size: %dx%d\n", _dev.width(), _dev.height());

    if (!_sprite.createSprite(_dev.width(), _dev.height())) {
        Serial.println("display::init | Failed to create sprite!");
        return false;
    }

    _sprite.setColorDepth(16);

    clear();

    return true;
}

void display::clear(int color)
{
    _sprite.fillScreen(color);
}

void display::show()
{
    std::lock_guard<std::mutex> lock(_menu_stack_mutex);

    if (_menu_stack.empty())
        return;

    clear();
    _menu_stack.top()->draw(_sprite);
    _sprite.pushSprite(0, 0);
}

void display::process_inputs()
{
    std::lock_guard<std::mutex> lock(_menu_stack_mutex);

    if (_menu_stack.empty())
        return;

    _menu_stack.top()->process_inputs();
}

void display::set_menu(menu * men)
{
    std::lock_guard<std::mutex> lock(_menu_stack_mutex);

    while (_menu_stack.size() > 0)
        _menu_stack.pop();

    _menu_stack.push(men);
}

void display::push_menu(menu * men)
{
    std::lock_guard<std::mutex> lock(_menu_stack_mutex);

    _menu_stack.push(men);
}
