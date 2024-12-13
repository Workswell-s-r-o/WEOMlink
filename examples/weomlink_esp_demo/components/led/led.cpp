#include "led.hpp"

#include "esp_log.h"
#include "led_strip.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "led";

Led::Led()
    : m_color(BLUE)
    , m_flashing(false)
    , m_refreshIntervalMs(200)
    , m_flashingIndicator(false)
{

}

Led& Led::instance()
{
    static Led instance;
    return instance;
}

void Led::task(void* arg)
{
    ESP_LOGI(TAG, "Task created.");
        
    led_strip_config_t strip_config = {};
    strip_config.strip_gpio_num = GPIO_NUM_45;
    strip_config.max_leds = 1;
    strip_config.led_pixel_format = LED_PIXEL_FORMAT_GRB; 
    strip_config.led_model = LED_MODEL_WS2812;
    strip_config.flags.invert_out = false;

    led_strip_rmt_config_t rmt_config = {};
    rmt_config.clk_src = RMT_CLK_SRC_DEFAULT;
    rmt_config.resolution_hz = 40e6;
    rmt_config.flags.with_dma = false;
    led_strip_handle_t ledStripHandle;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &ledStripHandle));

    Led& instance = Led::instance();

    while (true)
    {
        RGB color = instance.getColor();
        if (instance.getFlashing())
        {
            if (instance.m_flashingIndicator)
            {
                color = OFF;
            }
            instance.m_flashingIndicator = !instance.m_flashingIndicator;
        }
        ESP_ERROR_CHECK(led_strip_set_pixel(ledStripHandle, 0, color.R, color.G, color.B));
        ESP_ERROR_CHECK(led_strip_refresh(ledStripHandle));
        vTaskDelay(pdMS_TO_TICKS(instance.getRefreshIntervalMs()));
    }
}

void Led::setColor(RGB color)
{
    m_color = color;
}

Led::RGB Led::getColor() const
{
    return m_color;
}

void Led::setFlashing(bool flashing)
{
    m_flashing = flashing;
}

bool Led::getFlashing() const
{
    return m_flashing;
}

void Led::setRefreshIntervalMs(size_t refreshIntervalMs)
{
    m_refreshIntervalMs = refreshIntervalMs;
}

size_t Led::getRefreshIntervalMs() const
{
    return m_refreshIntervalMs;
}