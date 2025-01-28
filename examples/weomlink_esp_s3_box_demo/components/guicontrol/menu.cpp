#include "menu.hpp"

#include "menuitems.hpp"
#include "displaylockguard.hpp"

Menu::Menu(const std::vector<MenuLine>& menuLines)
    : m_menuLines(menuLines)
    , m_currentIndex(0)
    , m_lastScrollTimePoint(Clock::now())
{
    DisplayLockGuard lock;

    lv_obj_t* screen = lv_screen_active();
    const int32_t screenWidth = lv_obj_get_width(screen);
    const int32_t screenHeight = lv_obj_get_height(screen);

    m_menuRootObject = lv_obj_create(screen);
    lv_obj_set_size(m_menuRootObject, screenWidth, screenHeight);
    lv_obj_center(m_menuRootObject);
    lv_obj_set_layout(m_menuRootObject, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(m_menuRootObject, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(m_menuRootObject, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(m_menuRootObject, 0, LV_STATE_DEFAULT);

    m_panelRoot = lv_obj_create(m_menuRootObject);
    lv_obj_set_size(m_panelRoot, lv_pct(100), lv_pct(80));
    lv_obj_set_layout(m_panelRoot, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(m_panelRoot, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_snap_x(m_panelRoot, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_flag(m_panelRoot, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_set_style_border_width(m_panelRoot, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(m_panelRoot, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(m_panelRoot, [](lv_event_t* e)
    {
        auto panel = (lv_obj_t*)lv_event_get_target_obj(e);
        lv_area_t panelArea;
        lv_obj_get_coords(panel, &panelArea);
        int panelCenterX = (panelArea.x1 + panelArea.x2) / 2;
        int panelCenterY = (panelArea.y1 + panelArea.y2) / 2;
        for (uint32_t i = 0; i < lv_obj_get_child_count(panel); ++i)
        {
            auto child = lv_obj_get_child(panel, i);
            lv_area_t childArea;
            lv_obj_get_coords(child, &childArea);

            if (childArea.x1 < panelCenterX && childArea.x2 > panelCenterX &&
                childArea.y1 < panelCenterY && childArea.y2 > panelCenterY)
            {
                ((Menu*)lv_event_get_user_data(e))->m_currentIndex = i;
                break;
            }
        }

    }, LV_EVENT_SCROLL_END, this);


    for (int row = 0; row < menuLines.size(); ++row)
    {
        auto* panel = lv_obj_create(m_panelRoot);
        lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_size(panel, lv_pct(75), lv_pct(100));

        const auto& line = menuLines.at(row);
        lv_obj_t* label = createLabel(panel, line);
        lv_obj_t* object = line.item->initializeGui(panel);
        lv_obj_set_size(object, lv_pct(100), lv_pct(75));
        lv_obj_center(object);
    }

    lv_obj_scroll_to_view(lv_obj_get_child(m_panelRoot, m_currentIndex), LV_ANIM_OFF);

    auto* scrollLeftButton = lv_obj_create(m_menuRootObject);
    lv_obj_add_flag(scrollLeftButton, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(scrollLeftButton, lv_pct(15), lv_obj_get_height(m_panelRoot));
    lv_obj_set_pos(scrollLeftButton, 0, 0);
    lv_obj_set_style_bg_opa(scrollLeftButton, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scrollLeftButton, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(scrollLeftButton, [](lv_event_t* e)
    {
        ((Menu*)lv_event_get_user_data(e))->previous();
    }, LV_EVENT_CLICKED, this);
    
    auto* scrollRightButton = lv_obj_create(m_menuRootObject);
    lv_obj_add_flag(scrollRightButton, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(scrollRightButton, lv_pct(15), lv_obj_get_height(m_panelRoot));
    lv_obj_set_pos(scrollRightButton, lv_pct(85), 0);
    lv_obj_set_style_bg_opa(scrollRightButton, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scrollRightButton, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(scrollRightButton, [](lv_event_t* e)
    {
        ((Menu*)lv_event_get_user_data(e))->next();
    }, LV_EVENT_CLICKED, this);

    auto* buttonRow = lv_obj_create(m_menuRootObject);
    lv_obj_set_size(buttonRow, lv_pct(100), lv_pct(20));
    lv_obj_set_style_pad_top(buttonRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(buttonRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_layout(buttonRow, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(buttonRow, LV_FLEX_FLOW_ROW);

    auto* cancelButton = lv_button_create(buttonRow);
    lv_obj_set_size(cancelButton, lv_pct(48), lv_pct(100));
    auto* cancelButtonLabel = lv_label_create(cancelButton);
    lv_label_set_text(cancelButtonLabel, "Cancel");
    lv_obj_center(cancelButtonLabel);
    lv_obj_add_event_cb(cancelButton, [](lv_event_t* e)
    {
        ((Menu*)lv_event_get_user_data(e))->cancel();
    }, LV_EVENT_CLICKED, this);

    auto* applyButton = lv_button_create(buttonRow);
    lv_obj_set_size(applyButton, lv_pct(48), lv_pct(100));
    auto* applyButtonLabel = lv_label_create(applyButton);
    lv_label_set_text(applyButtonLabel, "Apply");
    lv_obj_center(applyButtonLabel);
    lv_obj_add_event_cb(applyButton, [](lv_event_t* e)
    {
        ((Menu*)lv_event_get_user_data(e))->apply();
    }, LV_EVENT_CLICKED, this);
}

Menu::~Menu()
{
    if (m_menuRootObject)
    {
        DisplayLockGuard lock;
        lv_obj_del(m_menuRootObject);
    }
}

lv_obj_t* Menu::createLabel(lv_obj_t* parent, const MenuLine& line)
{
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text_fmt(label, "%s:", line.title.c_str());
    lv_obj_center(label);
    lv_obj_set_width(label, lv_pct(100));
    return label;
}

void Menu::next()
{
    TimePoint now = Clock::now();
    if (m_currentIndex < m_menuLines.size() - 1 && (now - m_lastScrollTimePoint) > SCROLL_DELAY_TIME) 
    {
        m_currentIndex += 1;
        lv_obj_scroll_to_view(lv_obj_get_child(m_panelRoot, m_currentIndex), LV_ANIM_ON);
        m_lastScrollTimePoint = now;
    }
}

void Menu::previous()
{
    TimePoint now = Clock::now();
    if (m_currentIndex > 0 && (now - m_lastScrollTimePoint) > SCROLL_DELAY_TIME)
    {
        m_currentIndex -= 1;
        lv_obj_scroll_to_view(lv_obj_get_child(m_panelRoot, m_currentIndex), LV_ANIM_ON);
        m_lastScrollTimePoint = now;
    }
}

void Menu::apply()
{
    m_menuLines.at(m_currentIndex).item->actionApply();
}

void Menu::cancel()
{
    m_menuLines.at(m_currentIndex).item->actionCancel();
}