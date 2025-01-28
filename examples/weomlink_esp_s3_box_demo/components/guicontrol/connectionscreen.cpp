#include "connectionscreen.hpp"

#include "displaylockguard.hpp"
#include "menuitems.hpp"

#include <vector>
#include <string>

ConnectionScreen::ConnectionScreen(const std::function<void(int)>& connnectCallback)
    : m_connectCallback(connnectCallback)
    , m_baudrate(115200)
{
    DisplayLockGuard lock;
    lv_obj_t* screen = lv_screen_active();
    const int screeenWidth = lv_obj_get_width(screen);
    const int screenHeight = lv_obj_get_height(screen);
    
    static constexpr std::array<int32_t, 5> rowDescription = {10, 70, 70, 90, LV_GRID_TEMPLATE_LAST};
    static constexpr std::array<int32_t, 3> columnDescription = {150, 150, LV_GRID_TEMPLATE_LAST};

    m_rootObject = lv_obj_create(screen);
    lv_obj_set_style_grid_column_dsc_array(m_rootObject, columnDescription.data(), 0);
    lv_obj_set_style_grid_row_dsc_array(m_rootObject, rowDescription.data(), 0);
    lv_obj_set_size(m_rootObject, screeenWidth, screenHeight);
    lv_obj_center(m_rootObject);
    lv_obj_set_layout(m_rootObject, LV_LAYOUT_GRID);
    
    auto* baudrateLabel = lv_label_create(m_rootObject);
    lv_label_set_text(baudrateLabel, "Baudrate:");
    lv_obj_set_grid_cell(baudrateLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    auto* baudrateDropdown = lv_dropdown_create(m_rootObject);
    lv_dropdown_set_options(baudrateDropdown, "115200\n"
                                                "921600\n"
                                                "3000000");
    const auto baudrateCb = [](lv_event_t* e)
    {
        lv_obj_t* dropdown = (lv_obj_t*)lv_event_get_target(e);
        uint32_t index = lv_dropdown_get_selected(dropdown);

        auto connectionScreen = (ConnectionScreen*)lv_event_get_user_data(e);
        switch (index)
        {
            case 0:
                connectionScreen->m_baudrate = 115200;
                break;
            case 1:
                connectionScreen->m_baudrate = 921600;
                break;
            case 2:
                connectionScreen->m_baudrate = 3000000;
                break;
            default:
                assert(false);
        }
    };
    lv_obj_add_event_cb(baudrateDropdown, baudrateCb, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_set_grid_cell(baudrateDropdown, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    auto* connectButton = lv_button_create(m_rootObject);
    static constexpr auto connectButtonCB = [](lv_event_t* e)
    {
        auto connectionScreen = (ConnectionScreen*)lv_event_get_user_data(e);
        connectionScreen->m_connectCallback(connectionScreen->m_baudrate);
    };
    lv_obj_add_event_cb(connectButton, connectButtonCB, LV_EVENT_CLICKED, this);
    lv_obj_t* buttonLabel = lv_label_create(connectButton);
    lv_label_set_text(buttonLabel, "Connect");
    lv_obj_set_grid_cell(connectButton, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 2, 1);
}

ConnectionScreen::~ConnectionScreen()
{
    if (m_rootObject)
    {
        DisplayLockGuard lock;
        lv_obj_del(m_rootObject);
    }
}
