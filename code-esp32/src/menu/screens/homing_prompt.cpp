/// homing_prompt.cpp

#include "../../main.hpp"

namespace m
{
    void homing_prompt::draw(LGFX_Sprite & sprite)
    {
        sprite.setTextSize(2.5);
        const auto h = int32_t(2.5 * sprite.fontHeight());
        const auto y = 20;

        sprite.setTextColor(TFT_BLACK);
        sprite.drawCenterString("Press", _CX, y);
        sprite.setTextColor(TFT_DARKRED);
        sprite.drawCenterString("ENTER", _CX, y + h);
        sprite.setTextColor(TFT_BLACK);
        sprite.drawCenterString("to begin homing.", _CX, y + h * 2);
        
        // --- Animation ---
        uint32_t t = millis();

        // Pulsing center dot
        int pulse = 4 + (sin(t / 250.0f) + 1.0f) * 3;   // size oscillates between 4–10
        sprite.fillCircle(_CX, y + h * 3.5, pulse, TFT_DARKCYAN);
    }

    void homing_prompt::process_inputs()
    {
        if (ctl._btn_enter)
            _continue = true;
    }
}
