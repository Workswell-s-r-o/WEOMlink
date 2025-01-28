#include "menu.hpp"

#include "menuitems.hpp"
#include "displaylockguard.hpp"

Menu::Menu(const std::vector<MenuLine>& menuLines)
    : m_menuLines(menuLines)
    , m_currentIndex(0)
{
    constexpr int32_t LINE_HEIGHT = 30;

    m_columnDescription = {LV_SIZE_CONTENT, LV_SIZE_CONTENT, LV_GRID_TEMPLATE_LAST};
    m_rowDescription.push_back(LV_GRID_TEMPLATE_LAST);
    
    DisplayLockGuard lock;

    lv_obj_t* screen = lv_screen_active();
    const int screeenWidth = lv_obj_get_width(screen);
    const int screenHeight = lv_obj_get_height(screen);

    m_menuRootObject = lv_obj_create(screen);
    lv_obj_set_style_grid_column_dsc_array(m_menuRootObject, m_columnDescription.data(), 0);
    lv_obj_set_size(m_menuRootObject, screeenWidth, screenHeight);
    lv_obj_center(m_menuRootObject);
    lv_obj_set_layout(m_menuRootObject, LV_LAYOUT_GRID);
    lv_obj_set_scroll_dir(m_menuRootObject, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(m_menuRootObject, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(m_menuRootObject, LV_SCROLLBAR_MODE_ON);

    for (int row = 0; row < m_menuLines.size(); ++row)
    {
        const auto& line = m_menuLines.at(row);
        lv_obj_t* label = createLabel(m_menuRootObject, line);
        m_lineTitleLabels.push_back(label);

        lv_obj_t* object = line.item->initializeGui(m_menuRootObject);
        
        m_rowDescription.insert(m_rowDescription.end() - 1, LINE_HEIGHT);

        lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_grid_cell(object, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(object, 150, LINE_HEIGHT);
    }

    lv_obj_set_style_grid_row_dsc_array(m_menuRootObject, m_rowDescription.data(), 0);
    lv_obj_scroll_to_view(m_lineTitleLabels.at(m_currentIndex), LV_ANIM_ON);
    lv_obj_set_state(m_menuLines.at(m_currentIndex).item->object(), LV_STATE_FOCUSED, true);
}

Menu::~Menu()
{
    if (m_menuRootObject)
    {
        DisplayLockGuard lock;
        lv_obj_del(m_menuRootObject);
    }
}

void Menu::invokeAction(GuiAction action)
{
    switch (action)
    {
        case GuiAction::NEXT:
        {
            m_menuLines.at(m_currentIndex).item->actionNext();
            break;
        }
        case GuiAction::PREVIOUS:
        {
            m_menuLines.at(m_currentIndex).item->actionPrevious();
            break;
        }
        case GuiAction::UP:
        {
            DisplayLockGuard lock;

            lv_obj_set_state(m_menuLines.at(m_currentIndex).item->object(), LV_STATE_FOCUSED, false);

            if (m_currentIndex > 0)
            {
                m_currentIndex -= 1;
            }

            lv_obj_scroll_to_view(m_lineTitleLabels.at(m_currentIndex), LV_ANIM_ON);
            lv_obj_set_state(m_menuLines.at(m_currentIndex).item->object(), LV_STATE_FOCUSED, true);
            break;
        }
        case GuiAction::DOWN:
        {
            DisplayLockGuard lock;

            lv_obj_set_state(m_menuLines.at(m_currentIndex).item->object(), LV_STATE_FOCUSED, false);

            if (m_currentIndex + 1 < m_menuLines.size())
            {
                m_currentIndex += 1;
            }

            lv_obj_scroll_to_view(m_lineTitleLabels.at(m_currentIndex), LV_ANIM_ON);
            lv_obj_set_state(m_menuLines.at(m_currentIndex).item->object(), LV_STATE_FOCUSED, true);
            break;
        }
        case GuiAction::APPLY:
        {
            m_menuLines.at(m_currentIndex).item->actionApply();
            break;
        }
        case GuiAction::CANCEL:
        {
            m_menuLines.at(m_currentIndex).item->actionCancel();
            break;
        }
    }
}

lv_obj_t* Menu::createLabel(lv_obj_t* parent, const MenuLine& line)
{
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text_fmt(label, "%s:", line.title.c_str());
    lv_obj_center(label);
    return label;
}
