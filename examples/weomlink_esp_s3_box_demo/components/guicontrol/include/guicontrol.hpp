#ifndef DISPLAYCONTROL_HPP
#define DISPLAYCONTROL_HPP

#include "uart.hpp"

#include "wl/weom.h"

#include "bsp/esp-bsp.h"

#include <string>
#include <memory>


class Menu;
class ConnectionScreen;

class GuiControl
{
public:
    GuiControl();

    static GuiControl& instance();

    void initialize();

private:
    void initializeDisplay();
    void initializeConnectionScreen();
    void initizalizeMenu();

    void connectWeom(int baudrate);

    void showError(const std::string& text);

    std::unique_ptr<Menu> m_menu;
    std::unique_ptr<ConnectionScreen> m_connectionScreen;

    wl::ContrastBrightness m_contrastBrightness;
    wl::ImageFlip m_imageFlip;
    wl::PresetId m_presetId;

    std::unique_ptr<wl::WEOM> m_coreControl;
};

#endif // DISPLAYCONTROL_HPP