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
    
    static constexpr int32_t LINE_HEIGHT = 30;
    static constexpr std::array<int32_t, 5> rowDescription = {LV_SIZE_CONTENT, LINE_HEIGHT, LINE_HEIGHT, LV_SIZE_CONTENT, LV_GRID_TEMPLATE_LAST};
    static constexpr std::array<int32_t, 3> columnDescription = {130, LV_SIZE_CONTENT, LV_GRID_TEMPLATE_LAST};

    m_rootObject = lv_obj_create(screen);
    lv_obj_set_style_grid_column_dsc_array(m_rootObject, columnDescription.data(), 0);
    lv_obj_set_style_grid_row_dsc_array(m_rootObject, rowDescription.data(), 0);
    lv_obj_set_size(m_rootObject, screeenWidth, screenHeight);
    lv_obj_center(m_rootObject);
    lv_obj_set_layout(m_rootObject, LV_LAYOUT_GRID);
    
    m_baudrateLabel = lv_label_create(m_rootObject);
    lv_label_set_text(m_baudrateLabel, "Baudrate:");
    lv_obj_set_grid_cell(m_baudrateLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    m_baudrateMenuItem = new ComboBoxMenuItem<3>(etl::make_vector<const char*>("115200", "921600", "3000000"), 0, [this](int index)
    {
        switch (index)
        {
            case 0:
                m_baudrate = 115200;
                break;
            case 1:
                m_baudrate = 921600;
                break;
            case 2:
                m_baudrate = 3000000;
                break;
            default:
                break;
        }
        return true;
    });
    lv_obj_t* baudrateItem = m_baudrateMenuItem->initializeGui(m_rootObject);
    lv_obj_set_grid_cell(baudrateItem, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_size(baudrateItem, 150, LINE_HEIGHT);

    m_connectButton = lv_button_create(m_rootObject);
    lv_obj_t* buttonLabel = lv_label_create(m_connectButton);
    lv_label_set_text(buttonLabel, "Connect");
    lv_obj_set_grid_cell(m_connectButton, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 2, 1);
}

ConnectionScreen::~ConnectionScreen()
{
    if (m_rootObject)
    {
        DisplayLockGuard lock;
        lv_obj_del(m_rootObject);
    }
}

void ConnectionScreen::invokeAction(GuiAction action)
{
    switch (action)
    {
        case GuiAction::NEXT:
            m_baudrateMenuItem->actionNext();
            m_baudrateMenuItem->actionApply();
            break;
        case GuiAction::PREVIOUS:
            m_baudrateMenuItem->actionPrevious();
            m_baudrateMenuItem->actionApply();
            break;
        case GuiAction::APPLY:
            m_connectCallback(m_baudrate);
            break;
        default:
            break;
    }
}