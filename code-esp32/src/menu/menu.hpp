/// menu.hpp

class menu
{
public:
    // Opposite since display rotated
    static constexpr auto _WIDTH = DISPLAY_HEIGHT;
    static constexpr auto _HEIGHT = DISPLAY_WIDTH;
    static constexpr auto _CX = DISPLAY_HEIGHT / 2;
    static constexpr auto _CY = DISPLAY_WIDTH / 2;

    menu();

    virtual void draw(LGFX_Sprite & sprite) = 0;
    virtual void process_inputs();
};
