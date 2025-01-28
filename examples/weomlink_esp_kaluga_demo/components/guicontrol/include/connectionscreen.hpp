#ifndef CONNECTIONSCREEN_HPP
#define CONNECTIONSCREEN_HPP

#include "guiaction.hpp"

#include "bsp/esp-bsp.h"

#include <functional>

class MenuItem;

class ConnectionScreen
{
public:
    ConnectionScreen(const std::function<void(int)>& connnectCallback);
    ~ConnectionScreen();

    void invokeAction(GuiAction action);

private:
    std::function<void(int)> m_connectCallback;
    int m_baudrate;

    lv_obj_t* m_rootObject;
    lv_obj_t* m_connectButton;
    lv_obj_t* m_connectingLabel;
    lv_obj_t* m_baudrateLabel;
    MenuItem* m_baudrateMenuItem;
};

#endif // CONNECTIONSCREEN_HPP