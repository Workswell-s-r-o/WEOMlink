#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "bsp/esp-bsp.h"

#include <array>

class Buttons
{
    Buttons();
public:
    static Buttons& instance();

    using Callback = void (*)(void*, void* arg);
    void registerCallback(bsp_button_t button, button_event_t event, Callback callback, void* arg = nullptr);

private:
    button_handle_t getButtonHandle(bsp_button_t button) const;

    std::array<button_handle_t, BSP_BUTTON_NUM> m_buttonHandles;
};

#endif // BUTTONS_HPP
