#ifndef CONNECTIONSCREEN_HPP
#define CONNECTIONSCREEN_HPP

#include "bsp/esp-bsp.h"

#include <functional>

class MenuItem;

class ConnectionScreen
{
public:
    ConnectionScreen(const std::function<void(int)>& connnectCallback);
    ~ConnectionScreen();

private:
    std::function<void(int)> m_connectCallback;
    int m_baudrate;

    lv_obj_t* m_rootObject;
};

#endif // CONNECTIONSCREEN_HPP