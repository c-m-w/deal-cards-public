/// display.hpp

class display
{
private:

    const int _BKG_CLR = TFT_LIGHTBLUE;
    const int _TXT_CLR = TFT_BLACK;

    lgfx_device _dev;
    LGFX_Sprite _sprite;
    std::stack<menu *> _menu_stack;
    std::mutex _menu_stack_mutex;

public:

    display(int pin_clock, int pin_data, int pin_rst, int pin_dc, int pin_cs);
    ~display() = default;

    bool init();
    void clear(int color = TFT_BLANCHEDALMOND);
    void show();
    void process_inputs();
    void set_menu(menu * men);
    void push_menu(menu * men);
};