#include "guicontrol.hpp"

#include "menuitems.hpp"
#include "displaylockguard.hpp"
#include "menu.hpp"
#include "connectionscreen.hpp"

#include "wl/dataclasses/firmwareversion.h"
#include "wl/dataclasses/contrastbrightness.h"

#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include <format>

static const char* TAG = "gui_control";

GuiControl::GuiControl()
    : m_coreControl(nullptr)
{

}

GuiControl& GuiControl::instance()
{
    static GuiControl instance;
    return instance;
}

void GuiControl::initialize()
{
    initializeDisplay();
    initializeConnectionScreen();
}

void GuiControl::initializeDisplay()
{
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = {
            .task_priority = 4,
            .task_stack = 16384,
            .task_affinity = -1,
            .task_max_sleep_ms = 500,
            .timer_period_ms = 5,
        },
        .buffer_size = BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT,
        .double_buffer = 0,
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
        }
    };
    lv_display_t* display = bsp_display_start_with_config(&cfg);
    assert(display);
    ESP_ERROR_CHECK(bsp_display_backlight_on());
}

void GuiControl::initizalizeMenu()
{
    assert(m_coreControl);
    ESP_LOGI(TAG, "Creating menu");
    
    auto contrastBrightness = m_coreControl->getMgcContrastBrightness();
    if (!contrastBrightness.has_value())
    {
        ESP_LOGE(TAG, "Failed to read contrast and brightness (%s)", contrastBrightness.error().c_str());
        return;
    }

    m_contrastBrightness = contrastBrightness.value();

    auto imageFlip = m_coreControl->getImageFlip();
    if (!imageFlip.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image flip (%s)", imageFlip.error().c_str());
        return;
    }

    m_imageFlip = imageFlip.value();

    auto serialNumber = m_coreControl->getSerialNumber();
    if (!serialNumber.has_value())
    {
        ESP_LOGE(TAG, "Failed to read serial number (%s)", serialNumber.error().c_str());
        return;
    }

    auto articleNumber = m_coreControl->getArticleNumber();
    if (!articleNumber.has_value())
    {
        ESP_LOGE(TAG, "Failed to read article number (%s)", articleNumber.error().c_str());
        return;
    }

    auto firmwareVersion = m_coreControl->getFirmwareVersion();
    if (!firmwareVersion.has_value())
    {
        ESP_LOGE(TAG, "Failed to read firmwareVersion (%s)", firmwareVersion.error().c_str());
        return;
    }
    
    auto paletteIndex = m_coreControl->getPaletteIndex();
    if (!paletteIndex.has_value())
    {
        ESP_LOGE(TAG, "Failed to read paletteIndex (%s)", paletteIndex.error().c_str());
        return;
    }

    auto frameRate = m_coreControl->getFramerate();
    if (!frameRate.has_value())
    {
        ESP_LOGE(TAG, "Failed to read frame rate (%s)", frameRate.error().c_str());
        return;
    }

    auto imageFreeze = m_coreControl->getImageFreeze();
    if (!imageFreeze.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image freeze (%s)", imageFreeze.error().c_str());
        return;
    }

    auto imageGenerator = m_coreControl->getImageGenerator();
    if (!imageGenerator.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image generator (%s)", imageGenerator.error().c_str());
        return;
    }

    auto shutterUpdateMode = m_coreControl->getShutterUpdateMode();
    if (!shutterUpdateMode.has_value())
    {
        ESP_LOGE(TAG, "Failed to read shutter update mode (%s)", shutterUpdateMode.error().c_str());
        return;
    }

    auto timeDomainAveraging = m_coreControl->getTimeDomainAveraging();
    if (!timeDomainAveraging.has_value())
    {
        ESP_LOGE(TAG, "Failed to read time domain averaging (%s)", timeDomainAveraging.error().c_str());
        return;
    }

    auto imageEqualizationType = m_coreControl->getImageEqualizationType();
    if (!imageEqualizationType.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image equalization type (%s)", imageEqualizationType.error().c_str());
        return;
    }

    auto agcNhSmoothingFrames = m_coreControl->getAgcNhSmoothingFrames();
    if (!agcNhSmoothingFrames.has_value())
    {
        ESP_LOGE(TAG, "Failed to read article AGC NH smoothing frames (%s)", agcNhSmoothingFrames.error().c_str());
        return;
    }

    auto spatialMedianFilterEnabled = m_coreControl->getSpatialMedianFilterEnabled();
    if (!spatialMedianFilterEnabled.has_value())
    {
        ESP_LOGE(TAG, "Failed to read spatial median filter enabled (%s)", spatialMedianFilterEnabled.error().c_str());
        return;
    }

    auto presetId = m_coreControl->getPresetId();
    if (!presetId.has_value())
    {
        ESP_LOGE(TAG, "Failed to read preset ID (%s)", presetId.error().c_str());
        return;
    }
    m_presetId = presetId.value();

    std::vector<MenuLine> menuDefinition = 
    {
        {"Serial number", std::make_shared<LabelMenuItem<wl::WEOM::SERIAL_NUMBER_STRING_SIZE>>(serialNumber.value().c_str())},
        {"Article number", std::make_shared<LabelMenuItem<wl::WEOM::SERIAL_NUMBER_STRING_SIZE>>(articleNumber.value().c_str())},
        {"FW version", std::make_shared<LabelMenuItem<wl::FirmwareVersion::MAXIMUM_STRING_SIZE>>(firmwareVersion.value().toString().c_str())},
        {"Palette", std::make_shared<SpinBoxMenuItem>(0, 15, 1, 
                                                      static_cast<int>(paletteIndex.value()), 
                                                      [this](int index) -> bool
                                                      {
                                                         return m_coreControl->setPaletteIndex(static_cast<uint8_t>(index));
                                                      })},
        {"Framerate", std::make_shared<ComboBoxMenuItem<3>>(etl::make_vector<const char*>("9 Hz", "30 Hz", "60 Hz"), 
                                                         static_cast<int>(frameRate.value()), 
                                                         [this](int index) -> bool
                                                         {
                                                            return m_coreControl->setFramerate(static_cast<wl::Framerate>(index));
                                                         })},
        {"Horizontal flip", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                          m_imageFlip.getHorizontalFlip() ? 1 : 0, 
                                                          [this](int index) -> bool
                                                          {
                                                              m_imageFlip.setHorizontalFlip(index == 1);
                                                              return m_coreControl->setImageFlip(m_imageFlip);
                                                          })},
        {"Vertical flip", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                          m_imageFlip.getVerticalFlip() ? 1 : 0, 
                                                          [this](int index) -> bool
                                                          {
                                                              m_imageFlip.setVerticalFlip(index == 1);
                                                              return m_coreControl->setImageFlip(m_imageFlip);
                                                          })},
        {"Image freeze", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                            imageFreeze.value() ? 1 : 0,  
                                                            [this](int index) -> bool
                                                            {
                                                               return m_coreControl->setImageFreeze(index == 1);
                                                            })},
        {"Image source", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("Sensor", "Pattern"), 
                                                            static_cast<int>(imageGenerator.value()), 
                                                            [this](int index) -> bool
                                                            {
                                                               return m_coreControl->setImageGenerator(static_cast<wl::ImageGenerator>(index));
                                                            })},
        {"NUC update mode", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("Periodic", "Adaptive"), 
                                                               static_cast<int>(shutterUpdateMode.value()) - 1, 
                                                               [this](int index) -> bool
                                                               {
                                                                  return m_coreControl->setShutterUpdateMode(static_cast<wl::ShutterUpdateMode>(index + 1));
                                                               })},
        {"Time domain average", std::make_shared<ComboBoxMenuItem<3>>(etl::make_vector<const char*>("OFF", "2 frames", "4 frames"), 
                                                                   static_cast<int>(timeDomainAveraging.value()),
                                                                   [this](int index) -> bool
                                                                   {
                                                                      return m_coreControl->setTimeDomainAveraging(static_cast<wl::TimeDomainAveraging>(index));
                                                                   })},
        {"Image equalization", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("AGC", "MGC"), 
                                                                  static_cast<int>(imageEqualizationType.value()),
                                                                  [this](int index) -> bool
                                                                  {
                                                                     return m_coreControl->setImageEqualizationType(static_cast<wl::ImageEqualizationType>(index));
                                                                  })},
        {"Contrast", std::make_shared<SpinBoxMenuItem>(0, 100, 10, 
                                                       static_cast<int>(m_contrastBrightness.getContrastPercent()), 
                                                       [this](int contrastPercent) -> bool
                                                       {
                                                           m_contrastBrightness.setContrastPercent(static_cast<float>(contrastPercent));
                                                           return m_coreControl->setMgcContrastBrightness(m_contrastBrightness);
                                                       })},
        {"Brightness", std::make_shared<SpinBoxMenuItem>(0, 100, 10, 
                                                         static_cast<int>(m_contrastBrightness.getBrightnessPercent()), 
                                                         [this](int brightnessPercent) -> bool
                                                         {
                                                             m_contrastBrightness.setBrightnessPercent(static_cast<float>(brightnessPercent));
                                                             return m_coreControl->setMgcContrastBrightness(m_contrastBrightness);
                                                         })},
        {"AGC NH smoothing", std::make_shared<ComboBoxMenuItem<5>>(etl::make_vector<const char*>("1 frame", "2 frames", "4 frames", "8 frames", "16 frames"), 
                                                                static_cast<int>(agcNhSmoothingFrames.value()),
                                                                [this](int index) -> bool
                                                                {
                                                                   return m_coreControl->setAgcNhSmoothingFrames(static_cast<uint8_t>(index));
                                                                })},
        {"Spatial median filter", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                                     spatialMedianFilterEnabled.value() ? 1 : 0,
                                                                     [this](int index) -> bool
                                                                     {
                                                                        return m_coreControl->setSpatialMedianFilterEnabled(index == 1);
                                                                     })},
                                                                             {"Range", std::make_shared<ComboBoxMenuItem<6>>(etl::make_vector<const char*>("Not defined", "R1", "R2", "R3", "High gain", "Low gain"),
                                                       static_cast<int>(m_presetId.getRange()),
                                                       [this](int index) -> bool
                                                       {
                                                            m_presetId.setRange(static_cast<wl::Range>(index));
                                                            return m_coreControl->setPresetId(m_presetId);
                                                       })},
        {"Lens", std::make_shared<ComboBoxMenuItem<7>>(etl::make_vector<const char*>("Not defined", "WTC 35", "WTC 25", "WTC 14", "WTC 7.5", "User 1", "User 2"),
                                                       static_cast<int>(m_presetId.getLens()),
                                                       [this](int index) -> bool
                                                       {
                                                            m_presetId.setLens(static_cast<wl::Lens>(index));
                                                            return m_coreControl->setPresetId(m_presetId);
                                                       })}
    };

    m_menu = std::make_unique<Menu>(menuDefinition);
}

void GuiControl::initializeConnectionScreen()
{
    ESP_LOGI(TAG, "Creating connection screen");
    m_connectionScreen = std::make_unique<ConnectionScreen>([this](int baudrate) { connectWeom(baudrate); });
}

void GuiControl::connectWeom(int baudrate)
{
    ESP_LOGI(TAG, "Connecting to WEOM at %i baud", baudrate);
    auto uart = Uart::connectUart(baudrate);
    if (uart)
    {
        auto coreControl = std::make_unique<wl::WEOM>([](const wl::Clock::duration& duration) { vTaskDelay(pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count())); });
        auto result = coreControl->setDataLinkInterface(etl::move(uart));
        if (result.has_value())
        {
            m_coreControl = std::move(coreControl);
            m_connectionScreen = nullptr;
            initizalizeMenu();
        }
        else
        {
            ESP_LOGW(TAG, "UART connected, but WEOM not responding");
            showError("Failed to connect");
        }
    }
    else
    {
        ESP_LOGW(TAG, "Failed to connect to UART");
        showError("Failed to connect");
    }
}

void GuiControl::showError(const std::string& text)
{
    DisplayLockGuard lock;
    lv_obj_t* msgBox = lv_msgbox_create(lv_screen_active());
    lv_obj_set_size(msgBox, lv_pct(80), lv_pct(80));
    lv_msgbox_add_title(msgBox, "Error");
    lv_msgbox_add_text(msgBox, text.c_str());
    lv_msgbox_add_close_button(msgBox);
}
