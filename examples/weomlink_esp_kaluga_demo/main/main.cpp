#include "guicontrol.hpp"
#include "led.hpp"

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Initialization started.");

    ESP_LOGI(TAG, "Creating 'led_task'.");
    xTaskCreate(&Led::task, "led_task", 2048, nullptr, configMAX_PRIORITIES - 1, nullptr);

    ESP_LOGI(TAG, "Initializing GUI");
    GuiControl::instance().initialize();
}
