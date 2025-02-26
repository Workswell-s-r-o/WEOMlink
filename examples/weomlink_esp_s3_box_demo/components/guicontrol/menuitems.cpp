#include "menuitems.hpp"

static const char* TAG = "menu_items";

MenuItem::MenuItem()
{
}

lv_obj_t* MenuItem::object() const
{
    return m_object;
}

lv_obj_t* MenuItem::initializeGui(lv_obj_t* parent)
{
    ESP_LOGI(TAG,"Calling initializeGui");
    m_parent = parent;
    ESP_LOGI(TAG,"Creating layout container object");
    m_object = lv_obj_create(parent);
    lv_obj_set_layout(m_object, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(m_object, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(m_object, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(m_object, LV_SCROLLBAR_MODE_OFF);
    ESP_LOGI(TAG,"Done creating layout container object");

    const lv_color_t editedColor = lv_palette_lighten(LV_PALETTE_AMBER, 3); 

    lv_style_init(&m_editedStyle);
    lv_style_set_bg_color(&m_editedStyle, editedColor);
    lv_obj_add_style(m_object, &m_editedStyle, LV_STATE_EDITED);
    lv_obj_add_style(m_object, &m_editedStyle, LV_STATE_EDITED | LV_STATE_FOCUSED);

    initializeGuiImpl();
    ESP_LOGI(TAG,"Done calling initializeGui");
    return m_object;
}

void MenuItem::actionNext()
{

}

void MenuItem::actionPrevious()
{

}

void MenuItem::actionApply()
{

}

void MenuItem::actionCancel()
{

}

TriggerMenuItem::TriggerMenuItem(const std::function<void(void)>& callback)
    : BaseClass()
    , m_callback(callback)
{
    ESP_LOGI(TAG, "Creating TriggerMenuItem");
}

void TriggerMenuItem::initializeGuiImpl()
{
    ESP_LOGI(TAG,"Creating trigger menu item");
    lv_obj_t* buttonLabel = lv_label_create(m_object);
    lv_label_set_text(buttonLabel, "Bang!");
    lv_obj_center(buttonLabel);
    lv_obj_set_flex_grow(buttonLabel, 1);
    ESP_LOGI(TAG,"Done creating trigger menu item");
}

void TriggerMenuItem::actionApply()
{
    m_callback();
}

SpinBoxMenuItem::SpinBoxMenuItem(int minimum, int maximum, int step, int initialValue, const std::function<bool(int)>& callback)
    : BaseClass()
    , m_minimum(minimum)
    , m_maximum(maximum)
    , m_step(step)
    , m_value(initialValue)
    , m_callback(callback)
    , m_editValue(initialValue)
    , m_lastChangeTimePoint(Clock::now())
{
    ESP_LOGI(TAG, "Creating SpinBoxMenuItem with initial value: %d",m_value);
}

void SpinBoxMenuItem::initializeGuiImpl()
{
    ESP_LOGI(TAG, "Creating SpinBoxMenuItem");
    ESP_LOGI(TAG, "Creating leftButton");
    m_leftButton = lv_button_create(m_object);
    lv_obj_set_size(m_leftButton, 30, 30);
    ESP_LOGI(TAG, "Done creating leftButton");
    ESP_LOGI(TAG, "Creating leftButtonCallback");
    static constexpr auto leftButtonCallback = [](lv_event_t* e)
    {
        ((SpinBoxMenuItem*)(lv_event_get_user_data(e)))->actionPrevious();
    };
    lv_obj_add_event_cb(m_leftButton, leftButtonCallback, LV_EVENT_CLICKED, this);
    ESP_LOGI(TAG, "Done creating leftButtonCallback");

    ESP_LOGI(TAG, "Creating leftButtonLabel");
    m_leftButtonlabel = lv_label_create(m_leftButton);
    lv_label_set_text(m_leftButtonlabel, "<");
    lv_obj_align(m_leftButtonlabel, LV_ALIGN_CENTER, 0, 0);
    ESP_LOGI(TAG, "Done creating leftButtonLabel");

    ESP_LOGI(TAG, "Creating m_valueLabel");
    m_valueLabel = lv_label_create(m_object);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_value);
    lv_obj_set_flex_grow(m_valueLabel, 1);
    ESP_LOGI(TAG, "Done creating m_valueLabel");

    ESP_LOGI(TAG, "Creating rightButton");
    m_rightButton = lv_button_create(m_object);
    lv_obj_set_size(m_rightButton, 30, 30);
    lv_obj_clear_flag(m_rightButton, LV_OBJ_FLAG_PRESS_LOCK);
    ESP_LOGI(TAG, "Done creating rightButton");

    ESP_LOGI(TAG, "Creating rightButtonCallback");
    static constexpr auto rightButtonCallback = [](lv_event_t* e)
    {
        ((SpinBoxMenuItem*)(lv_event_get_user_data(e)))->actionNext();
    };
    lv_obj_add_event_cb(m_rightButton, rightButtonCallback, LV_EVENT_CLICKED, this);
    ESP_LOGI(TAG, "Done creating rightButtonCallback");

    ESP_LOGI(TAG, "Creating rightButtonLabel");
    m_rightButtonlabel = lv_label_create(m_rightButton);
    lv_label_set_text(m_rightButtonlabel, ">");
    lv_obj_align(m_rightButtonlabel, LV_ALIGN_CENTER, 0, 0);
    ESP_LOGI(TAG, "Done creating rightButtonLabel");

    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
    ESP_LOGI(TAG, "Done creating SpinBoxMenuItem");
}

void SpinBoxMenuItem::updateGui()
{
    DisplayLockGuard lock;
    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editValue != m_value);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_editValue);
}

void SpinBoxMenuItem::actionNext()
{
    TimePoint now = Clock::now();
    if ((now - m_lastChangeTimePoint) < DEBOUNCE_DELAY_TIME) 
    {
        return;
    }
    m_lastChangeTimePoint = now;

    m_editValue = std::min(m_editValue + m_step, m_maximum);
    updateGui();
}

void SpinBoxMenuItem::actionPrevious()
{
    TimePoint now = Clock::now();
    if ((now - m_lastChangeTimePoint) < DEBOUNCE_DELAY_TIME) 
    {
        return;
    }
    m_lastChangeTimePoint = now;

    m_editValue = std::max(m_editValue - m_step, m_minimum);
    updateGui();
}

void SpinBoxMenuItem::actionApply()
{
    if (m_callback(m_editValue))
    {
        m_value = m_editValue;
    }
    else
    {
        m_editValue = m_value;
    }

    updateGui();
}

void SpinBoxMenuItem::actionCancel()
{
    m_editValue = m_value;
    
    updateGui();
}