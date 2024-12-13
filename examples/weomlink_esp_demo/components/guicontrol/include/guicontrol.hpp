#ifndef DISPLAYCONTROL_HPP
#define DISPLAYCONTROL_HPP

#include "uart.hpp"
#include "guiaction.hpp"

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

    enum class State
    {
        NOT_CONNECTED,
        CONNECTED,
        ERROR
    };

    void invokeAction(GuiAction action);
    void setState(State state);

private:
    void registerButtonCallbacks();
    void initializeDisplay();
    void initializeConnectionScreen();
    void initizalizeMenu();

    void connectWeom(int baudrate);

    void showError(const std::string& text);
    void closeErrorPopup();

    static const char* stateToString(State state);

    lv_obj_t* m_msgBox;

    std::unique_ptr<Menu> m_menu;
    std::unique_ptr<ConnectionScreen> m_connectionScreen;

    State m_currentState;
    State m_stateBeforeError;

    wl::ContrastBrightness m_contrastBrightness;
    wl::ImageFlip m_imageFlip;

    wl::WEOM m_coreControl;
};

#endif // DISPLAYCONTROL_HPP