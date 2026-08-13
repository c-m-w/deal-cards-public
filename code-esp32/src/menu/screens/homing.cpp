/// homing.cpp

#include "../../main.hpp"

namespace m
{
    void homing::draw(LGFX_Sprite & sprite)
    {
        // --- Animation ---
        uint32_t t = millis();

        // Outer arc: steady linear spin
        float outer_angle = (t % 3000) / 3000.0f * 360.0f;

        // Inner arcs: non-linear speed (fast → slow → fast)
        float phase = (t % 4500) / 4500.0f;
        float eased = 0.5f - 0.5f * cosf(phase * 2.0f * PI);
        float inner_angle = eased * 360.0f;

        float phase2 = (t % 5200) / 5200.0f;
        float eased2 = 0.5f - 0.5f * cosf(phase2 * 2.0f * PI);
        float brown_angle = eased2 * 360.0f;

        // Arc lengths that breathe over time
        float blue_len   = 70.0f + 50.0f * sinf(t / 700.0f);     // 70° → 120°
        float brown_len  = 55.0f + 45.0f * sinf(t / 900.0f + 1.2f); // 55° → 100°

        const int radius = min(DISPLAY_WIDTH, DISPLAY_HEIGHT) / 2 - 15;

        // Soft matte colors
        const uint16_t mattePurple = sprite.color565(95, 55, 145);   // richer violet
        const uint16_t matteBlue   = sprite.color565(40, 105, 155);  // deeper slate blue
        const uint16_t matteBrown  = sprite.color565(150, 95, 55);   // warm terracotta

        // Background faint ring
        sprite.fillArc(_CX, _CY, radius, radius - 18, 0, 360, sprite.color565(40, 40, 45));

        // Main thick spinning arc (purple) - linear + fixed length
        sprite.fillArc(_CX, _CY, radius, radius - 18, outer_angle, outer_angle + 130, mattePurple);

        // Secondary arc (blue) - non-linear + changing length
        sprite.fillArc(_CX, _CY, radius - 22, radius - 36,
                    inner_angle + 180,
                    inner_angle + 180 + blue_len,
                    matteBlue);

        // Inner accent arc (brown) - different non-linear timing + changing length
        sprite.fillArc(_CX, _CY, radius - 42, radius - 52,
                    brown_angle + 90,
                    brown_angle + 90 + brown_len,
                    matteBrown);
    }
}
