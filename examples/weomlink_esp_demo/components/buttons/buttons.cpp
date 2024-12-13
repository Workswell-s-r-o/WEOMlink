#include "buttons.hpp"

#include "esp_err.h"

Buttons::Buttons()
{
    int buttonsCount = 0;
    ESP_ERROR_CHECK(bsp_iot_button_create(m_buttonHandles.data(), &buttonsCount, BSP_BUTTON_NUM));
    assert(buttonsCount == BSP_BUTTON_NUM);
}

Buttons& Buttons::instance()
{
    static Buttons instance;
    return instance;
}

void Buttons::registerCallback(bsp_button_t button, button_event_t event, Callback callback, void* arg)
{
    ESP_ERROR_CHECK(iot_button_register_cb(getButtonHandle(button), event, callback, arg));
}

button_handle_t Buttons::getButtonHandle(bsp_button_t button) const
{
    return m_buttonHandles.at(static_cast<size_t>(button));
}
