#include "guicontrol.hpp"

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Initialization started.");

    ESP_LOGI(TAG, "Initializing GUI");
    GuiControl::instance().initialize();
}
