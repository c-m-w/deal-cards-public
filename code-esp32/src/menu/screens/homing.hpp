/// homing.hpp

namespace m
{
    class homing : public menu
    {
    public:

        void draw(LGFX_Sprite & sprite) override;
    };

    inline homing _homing;
}
