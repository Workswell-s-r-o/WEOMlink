#include "guicontrol.hpp"

#include "menuitems.hpp"
#include "displaylockguard.hpp"
#include "buttons.hpp"
#include "menu.hpp"
#include "connectionscreen.hpp"
#include "led.hpp"

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
    : m_msgBox(nullptr)
    , m_currentState(State::NOT_CONNECTED)
    , m_stateBeforeError(State::NOT_CONNECTED)
    , m_coreControl([](const wl::Clock::duration& duration) { vTaskDelay(pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count())); })
{

}

GuiControl& GuiControl::instance()
{
    static GuiControl instance;
    return instance;
}

void GuiControl::initialize()
{
    registerButtonCallbacks();
    initializeDisplay();
    initializeConnectionScreen();
}

void GuiControl::registerButtonCallbacks()
{
    auto buttons = Buttons::instance();

    buttons.registerCallback(BSP_BUTTON_VOLDOWN, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::UP);
    });

    buttons.registerCallback(BSP_BUTTON_VOLUP, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::DOWN);
    });

    buttons.registerCallback(BSP_BUTTON_MODE, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::PREVIOUS);
    });

    buttons.registerCallback(BSP_BUTTON_REC, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::NEXT);
    });

    buttons.registerCallback(BSP_BUTTON_PLAY, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::APPLY);
    });

    buttons.registerCallback(BSP_BUTTON_SET, BUTTON_PRESS_DOWN, [](void*, void*)
    {
        GuiControl::instance().invokeAction(GuiAction::CANCEL);
    });
}

void GuiControl::initializeDisplay()
{
    lv_display_t* display = bsp_display_start();
    assert(display);
    ESP_ERROR_CHECK(bsp_display_backlight_on());
}

void GuiControl::initizalizeMenu()
{
    ESP_LOGI(TAG, "Creating menu");

    auto contrastBrightness = m_coreControl.getMgcConstrastBrightness();
    if (!contrastBrightness.has_value())
    {
        ESP_LOGE(TAG, "Failed to read contrast and brightness (%s)", contrastBrightness.error().c_str());
        return;
    }

    m_contrastBrightness = contrastBrightness.value();

    auto imageFlip = m_coreControl.getImageFlip();
    if (!imageFlip.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image flip (%s)", imageFlip.error().c_str());
        return;
    }

    m_imageFlip = imageFlip.value();

    auto serialNumber = m_coreControl.getSerialNumber();
    if (!serialNumber.has_value())
    {
        ESP_LOGE(TAG, "Failed to read serial number (%s)", serialNumber.error().c_str());
        return;
    }

    auto articleNumber = m_coreControl.getArticleNumber();
    if (!articleNumber.has_value())
    {
        ESP_LOGE(TAG, "Failed to read article number (%s)", articleNumber.error().c_str());
        return;
    }

    auto firmwareVersion = m_coreControl.getFirmwareVersion();
    if (!firmwareVersion.has_value())
    {
        ESP_LOGE(TAG, "Failed to read firmwareVersion (%s)", firmwareVersion.error().c_str());
        return;
    }
    
    auto paletteIndex = m_coreControl.getPaletteIndex();
    if (!paletteIndex.has_value())
    {
        ESP_LOGE(TAG, "Failed to read paletteIndex (%s)", paletteIndex.error().c_str());
        return;
    }

    auto frameRate = m_coreControl.getFramerate();
    if (!frameRate.has_value())
    {
        ESP_LOGE(TAG, "Failed to read frame rate (%s)", frameRate.error().c_str());
        return;
    }

    auto imageFreeze = m_coreControl.getImageFreeze();
    if (!imageFreeze.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image freeze (%s)", imageFreeze.error().c_str());
        return;
    }

    auto imageGenerator = m_coreControl.getImageGenerator();
    if (!imageGenerator.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image generator (%s)", imageGenerator.error().c_str());
        return;
    }

    auto shutterUpdateMode = m_coreControl.getShutterUpdateMode();
    if (!shutterUpdateMode.has_value())
    {
        ESP_LOGE(TAG, "Failed to read shutter update mode (%s)", shutterUpdateMode.error().c_str());
        return;
    }

    auto timeDomainAveraging = m_coreControl.getTimeDomainAveraging();
    if (!timeDomainAveraging.has_value())
    {
        ESP_LOGE(TAG, "Failed to read time domain averaging (%s)", timeDomainAveraging.error().c_str());
        return;
    }

    auto imageEqualizationType = m_coreControl.getImageEqualizationType();
    if (!imageEqualizationType.has_value())
    {
        ESP_LOGE(TAG, "Failed to read image equalization type (%s)", imageEqualizationType.error().c_str());
        return;
    }

    auto agcNhSmoothingFrames = m_coreControl.getAgcNhSmoothingFrames();
    if (!agcNhSmoothingFrames.has_value())
    {
        ESP_LOGE(TAG, "Failed to read article AGC NH smoothing frames (%s)", agcNhSmoothingFrames.error().c_str());
        return;
    }

    auto spatialMedianFilterEnabled = m_coreControl.getSpatialMedianFilterEnabled();
    if (!spatialMedianFilterEnabled.has_value())
    {
        ESP_LOGE(TAG, "Failed to read spatial median filter enabled (%s)", spatialMedianFilterEnabled.error().c_str());
        return;
    }

    auto presetId = m_coreControl.getPresetId();
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
                                                         return m_coreControl.setPaletteIndex(static_cast<uint8_t>(index));
                                                      })},
        {"Framerate", std::make_shared<ComboBoxMenuItem<3>>(etl::make_vector<const char*>("9 Hz", "30 Hz", "60 Hz"), 
                                                         static_cast<int>(frameRate.value()), 
                                                         [this](int index) -> bool
                                                         {
                                                            return m_coreControl.setFramerate(static_cast<wl::Framerate>(index));
                                                         })},
        {"Horizontal flip", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                          m_imageFlip.getHorizontalFlip() ? 1 : 0, 
                                                          [this](int index) -> bool
                                                          {
                                                              m_imageFlip.setHorizontalFlip(index == 1);
                                                              return m_coreControl.setImageFlip(m_imageFlip);
                                                          })},
        {"Vertical flip", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                          m_imageFlip.getVerticalFlip() ? 1 : 0, 
                                                          [this](int index) -> bool
                                                          {
                                                              m_imageFlip.setVerticalFlip(index == 1);
                                                              return m_coreControl.setImageFlip(m_imageFlip);
                                                          })},
        {"Image freeze", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                            imageFreeze.value() ? 1 : 0,  
                                                            [this](int index) -> bool
                                                            {
                                                               return m_coreControl.setImageFreeze(index == 1);
                                                            })},
        {"Image source", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("Sensor", "Pattern"), 
                                                            static_cast<int>(imageGenerator.value()), 
                                                            [this](int index) -> bool
                                                            {
                                                               return m_coreControl.setImageGenerator(static_cast<wl::ImageGenerator>(index));
                                                            })},
        {"NUC update mode", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("Periodic", "Adaptive"), 
                                                               static_cast<int>(shutterUpdateMode.value()) - 1, 
                                                               [this](int index) -> bool
                                                               {
                                                                  return m_coreControl.setShutterUpdateMode(static_cast<wl::ShutterUpdateMode>(index + 1));
                                                               })},
        {"Time domain average", std::make_shared<ComboBoxMenuItem<3>>(etl::make_vector<const char*>("OFF", "2 frames", "4 frames"), 
                                                                   static_cast<int>(timeDomainAveraging.value()),
                                                                   [this](int index) -> bool
                                                                   {
                                                                      return m_coreControl.setTimeDomainAveraging(static_cast<wl::TimeDomainAveraging>(index));
                                                                   })},
        {"Image equalization", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("AGC", "MGC"), 
                                                                  static_cast<int>(imageEqualizationType.value()),
                                                                  [this](int index) -> bool
                                                                  {
                                                                     return m_coreControl.setImageEqualizationType(static_cast<wl::ImageEqualizationType>(index));
                                                                  })},
        {"Contrast", std::make_shared<SpinBoxMenuItem>(0, 100, 10, 
                                                       static_cast<int>(m_contrastBrightness.getContrastPercent()), 
                                                       [this](int contrastPercent) -> bool
                                                       {
                                                           m_contrastBrightness.setContrastPercent(static_cast<float>(contrastPercent));
                                                           return m_coreControl.setMgcContrastBrightness(m_contrastBrightness);
                                                       })},
        {"Brightness", std::make_shared<SpinBoxMenuItem>(0, 100, 10, 
                                                         static_cast<int>(m_contrastBrightness.getBrightnessPercent()), 
                                                         [this](int brightnessPercent) -> bool
                                                         {
                                                             m_contrastBrightness.setBrightnessPercent(static_cast<float>(brightnessPercent));
                                                             return m_coreControl.setMgcContrastBrightness(m_contrastBrightness);
                                                         })},
        {"AGC NH smoothing", std::make_shared<ComboBoxMenuItem<5>>(etl::make_vector<const char*>("1 frame", "2 frames", "4 frames", "8 frames", "16 frames"), 
                                                                static_cast<int>(agcNhSmoothingFrames.value()),
                                                                [this](int index) -> bool
                                                                {
                                                                   return m_coreControl.setAgcNhSmoothingFrames(static_cast<uint8_t>(index));
                                                                })},
        {"Spatial median filter", std::make_shared<ComboBoxMenuItem<2>>(etl::make_vector<const char*>("OFF", "ON"), 
                                                                     spatialMedianFilterEnabled.value() ? 1 : 0,
                                                                     [this](int index) -> bool
                                                                     {
                                                                        return m_coreControl.setSpatialMedianFilterEnabled(index == 1);
                                                                     })},
        {"Range", std::make_shared<ComboBoxMenuItem<6>>(etl::make_vector<const char*>("Not defined", "R1", "R2", "R3", "High gain", "Low gain"),
                                                       static_cast<int>(m_presetId.getRange()),
                                                       [this](int index) -> bool
                                                       {
                                                            m_presetId.setRange(static_cast<wl::Range>(index));
                                                            return m_coreControl.setPresetId(m_presetId);
                                                       })},
        {"Lens", std::make_shared<ComboBoxMenuItem<7>>(etl::make_vector<const char*>("Not defined", "WTC 35", "WTC 25", "WTC 14", "WTC 7.5", "User 1", "User 2"),
                                                       static_cast<int>(m_presetId.getLens()),
                                                       [this](int index) -> bool
                                                       {
                                                            m_presetId.setLens(static_cast<wl::Lens>(index));
                                                            return m_coreControl.setPresetId(m_presetId);
                                                       })},
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
    Led::instance().setFlashing(true);
    auto uart = Uart::connectUart(baudrate);
    if (uart)
    {
        auto result = m_coreControl.setDataLinkInterface(etl::move(uart));
        if (result.has_value())
        {
            setState(State::CONNECTED);
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
    if (m_msgBox)
    {
        return;
    }

    setState(State::ERROR);

    DisplayLockGuard lock;
    m_msgBox = lv_msgbox_create(lv_screen_active());
    lv_msgbox_add_footer_button(m_msgBox, "Close");
    lv_msgbox_add_title(m_msgBox, "Error");
    lv_msgbox_add_text(m_msgBox, text.c_str());
}

void GuiControl::closeErrorPopup()
{
    if (!m_msgBox)
    {
        return;
    }

    setState(m_stateBeforeError);

    DisplayLockGuard lock;
    lv_msgbox_close(m_msgBox);
    m_msgBox = nullptr;
}

void GuiControl::invokeAction(GuiAction action)
{
    switch (m_currentState)
    {   
        case State::NOT_CONNECTED:
        {
            m_connectionScreen->invokeAction(action);
            break;
        }

        case State::CONNECTED:
        {
            m_menu->invokeAction(action);
            break;
        }

        case State::ERROR:
        {
            switch (action)
            {
                case GuiAction::APPLY:
                case GuiAction::CANCEL:
                {
                    closeErrorPopup();
                    break;
                }
                default:
                {
                    ESP_LOGI(TAG, "Action '%s' unhandled in state '%s'", guiActionToString(action), stateToString(m_currentState));
                    break;
                }
            }
            break;
        }

    }
}

void GuiControl::setState(State state)
{
    if (m_currentState == state)
    {
        return;
    }
    ESP_LOGI(TAG, "Switching state from '%s' to '%s'", stateToString(m_currentState), stateToString(state));
    switch (state)
    {
        case State::NOT_CONNECTED:
        {
            if (m_currentState != State::ERROR)
            {
                m_menu = nullptr;
                initializeConnectionScreen();
            }
            Led::instance().setColor(Led::BLUE);
            Led::instance().setFlashing(false);
            break;
        }
        case State::CONNECTED:
        {
            if (m_currentState != State::ERROR)
            {
                m_connectionScreen = nullptr;
                initizalizeMenu();
            }
            Led::instance().setColor(Led::GREEN);
            Led::instance().setFlashing(false);
            break;
        }
        case State::ERROR:
        {
            m_stateBeforeError = m_currentState;
            Led::instance().setColor(Led::RED);
            Led::instance().setFlashing(true);
            break;
        }
    }
    m_currentState = state;
}
    
const char* GuiControl::stateToString(State state)
{
    switch (state)
    {
        case State::NOT_CONNECTED:
            return "NOT CONNECTED";
        case State::CONNECTED:
            return "CONNECTED";
        case State::ERROR:
            return "ERROR";
    }
    assert(false);
    return "";
}