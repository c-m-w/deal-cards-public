/// tune_pid.cpp

#include "../../main.hpp"

namespace m
{
    void tune_pid::draw(LGFX_Sprite & sprite)
    {
        static const auto str_ctl = "Des. spd: ";
        static const auto w_ctl = sprite.textWidth(str_ctl);
        static const auto str_rob = "Cur. pos: ";
        static const auto w_rob = sprite.textWidth(str_rob);

        auto const ctl_speed = ctl.get_ctl_speed();
        auto const rob_angle = rob._actrl.angle();
        
        sprite.setTextSize(3.0);
        sprite.setTextColor(TFT_BLACK);
        sprite.drawCenterString("PID Tuning", _CX, 10);

        sprite.setTextSize(2);
        auto const h = int32_t(1.5 * sprite.fontHeight());
        auto const y0 = 40;

        std::array<std::pair<std::string, std::optional<std::string>>, N_ENTRIES> lines
        {
            std::pair { "Kp:", helpers::float_to_string(rob._pid._K_p, 5) },
            std::pair { "Ki:", helpers::float_to_string(rob._pid._K_i, 5) },
            std::pair { "Kd:", helpers::float_to_string(rob._pid._K_d, 5) },
            std::pair { "Kff:", helpers::float_to_string(rob._pid._K_ff, 5) },
            std::pair { "Cur. pos:", helpers::float_to_string(rob_angle, 1) }
        };

        for (auto i = 0; i < lines.size(); i++)
        {
            auto const & pair = lines[i];
            auto const cur_h = y0 + i * h;

            if (_selected_idx == i)
            {
                sprite.drawString(">", 4, cur_h);
            }

            if (pair.second.has_value())
            {
                sprite.drawString(pair.first.c_str(), 16, cur_h);
                sprite.drawRightString(pair.second.value().c_str(), _WIDTH - 4, cur_h);
            }
            else
            {
                sprite.drawCenterString(pair.first.c_str(), _CX, cur_h);
            }
        }
    }

    void tune_pid::process_inputs()
    {
        if (ctl._btn_dn)
        {
            while (ctl._btn_dn)
                delay(1);
            
            _selected_idx = (_selected_idx + 1) % N_ENTRIES;
            delay(200);
        }

        if (ctl._btn_up)
        {
            while (ctl._btn_up)
                delay(1);
            
            _selected_idx = (_selected_idx - 1 + N_ENTRIES) % N_ENTRIES;
            delay(200);
        }

        if (_selected_idx == 0)
            rob._pid._K_p -= ctl.get_ctl_speed() / 10000.f;
        if (_selected_idx == 1)
            rob._pid._K_i -= ctl.get_ctl_speed() / 10000.f;
        if (_selected_idx == 2)
            rob._pid._K_d -= ctl.get_ctl_speed() / 10000.f;
        if (_selected_idx == 3)
            rob._pid._K_ff -= ctl.get_ctl_speed() / 10000.f;
        if (_selected_idx == 4)
            rob.move(ctl.get_ctl_speed());
        if (_selected_idx == 4 && ctl._btn_enter)
        {
            rob.move_to(180.f);
        }
    }
}
