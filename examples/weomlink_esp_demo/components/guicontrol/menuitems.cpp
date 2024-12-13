#include "menuitems.hpp"

#include "esp_log.h"

MenuItem::MenuItem()
{
}

lv_obj_t* MenuItem::object() const
{
    return m_object;
}

lv_obj_t* MenuItem::initializeGui(lv_obj_t* parent)
{
    m_parent = parent;
    m_object = lv_obj_create(parent);
    lv_obj_set_layout(m_object, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(m_object, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(m_object, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(m_object, LV_SCROLLBAR_MODE_OFF);

    const lv_color_t focusedColor = lv_theme_get_color_primary(m_parent);
    const lv_color_t editedColor = lv_palette_lighten(LV_PALETTE_AMBER, 3); 

    lv_style_init(&m_focusedStyle);
    lv_style_set_border_color(&m_focusedStyle, focusedColor);
    lv_obj_add_style(m_object, &m_focusedStyle, LV_STATE_FOCUSED);

    lv_style_init(&m_editedStyle);
    lv_style_set_bg_color(&m_editedStyle, editedColor);
    lv_obj_add_style(m_object, &m_editedStyle, LV_STATE_EDITED);

    lv_style_init(&m_focusedAndEditedStyle);
    lv_style_set_border_color(&m_focusedAndEditedStyle, focusedColor);
    lv_style_set_bg_color(&m_focusedAndEditedStyle, editedColor);
    lv_obj_add_style(m_object, &m_focusedAndEditedStyle, LV_STATE_FOCUSED | LV_STATE_EDITED);

    initializeGuiImpl();
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

}

void TriggerMenuItem::initializeGuiImpl()
{
    lv_obj_t* buttonLabel = lv_label_create(m_object);
    lv_label_set_text(buttonLabel, "Bang!");
    lv_obj_center(buttonLabel);
    lv_obj_set_flex_grow(buttonLabel, 1);
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
{

}

void SpinBoxMenuItem::initializeGuiImpl()
{
    m_leftButton = lv_button_create(m_object);
    lv_obj_set_size(m_leftButton, 20, 20);

    m_leftButtonlabel = lv_label_create(m_leftButton);
    lv_label_set_text(m_leftButtonlabel, "<");
    lv_obj_align(m_leftButtonlabel, LV_ALIGN_CENTER, 0, 0);

    m_valueLabel = lv_label_create(m_object);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_value);
    lv_obj_set_flex_grow(m_valueLabel, 1);

    m_rightButton = lv_button_create(m_object);
    lv_obj_set_size(m_rightButton, 20, 20);

    m_rightButtonlabel = lv_label_create(m_rightButton);
    lv_label_set_text(m_rightButtonlabel, ">");
    lv_obj_align(m_rightButtonlabel, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
}

void SpinBoxMenuItem::actionNext()
{
    m_editValue = std::min(m_editValue + m_step, m_maximum);

    DisplayLockGuard lock;
    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editValue != m_value);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_editValue);
}

void SpinBoxMenuItem::actionPrevious()
{
    m_editValue = std::max(m_editValue - m_step, m_minimum);

    DisplayLockGuard lock;
    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editValue != m_value);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_editValue);
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

    DisplayLockGuard lock;
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editValue != m_value);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_editValue);
}

void SpinBoxMenuItem::actionCancel()
{
    m_editValue = m_value;
    
    DisplayLockGuard lock;
    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editValue == m_minimum);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editValue == m_maximum);
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editValue != m_value);
    lv_label_set_text_fmt(m_valueLabel, "%d", m_editValue);
}