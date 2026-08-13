/// lgfx_device.hpp

#include <LovyanGFX.hpp>

class lgfx_device : public lgfx::LGFX_Device
{
private:

    //const int _SPI_FREQ = 40'000'000;
    const int _SPI_FREQ = 39'000'000;

    lgfx::Panel_ST7789 _panel;
    lgfx::Bus_SPI      _bus;

public:

    lgfx_device(int pin_clock, int pin_data, int pin_rst, int pin_dc, int pin_cs) 
    {
        auto cfg = _bus.config();
        cfg.spi_host   = SPI2_HOST;
        cfg.freq_write = _SPI_FREQ;
        cfg.pin_sclk   = pin_clock;
        cfg.pin_mosi   = pin_data;
        cfg.pin_dc     = pin_dc;
        cfg.pin_miso   = -1;
        _bus.config(cfg);
        _panel.setBus(&_bus);

        auto panel_cfg = _panel.config();
        panel_cfg.pin_cs   = pin_cs;
        panel_cfg.pin_rst  = pin_rst;
        panel_cfg.panel_width  = DISPLAY_WIDTH;
        panel_cfg.panel_height = DISPLAY_HEIGHT;
        panel_cfg.rgb_order = false;
        panel_cfg.invert    = true;
        _panel.config(panel_cfg);
        setPanel(&_panel);
    }
};
