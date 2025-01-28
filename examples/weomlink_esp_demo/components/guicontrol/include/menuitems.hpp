#ifndef MENUITEMS_HPP
#define MENUITEMS_HPP

#include "displaylockguard.hpp"

#include "bsp/esp-bsp.h"

#include <etl/string.h>
#include <etl/vector.h>

#include <functional>
#include <stddef.h>

class MenuItem
{
public:    
    MenuItem();

    lv_obj_t* object() const;

    lv_obj_t* initializeGui(lv_obj_t* parent);

    virtual void actionNext();
    virtual void actionPrevious();
    virtual void actionApply();
    virtual void actionCancel();

protected:
    virtual void initializeGuiImpl() = 0;

    lv_obj_t* m_object;
    
private:
    lv_obj_t* m_parent;
    lv_style_t m_focusedStyle;
    lv_style_t m_editedStyle;
    lv_style_t m_focusedAndEditedStyle;
};

template <size_t size>
class LabelMenuItem : public MenuItem
{
    using BaseClass = MenuItem;
public:
    LabelMenuItem(const etl::string<size>& text);

protected:
    virtual void initializeGuiImpl() override;

private:
    etl::string<size> m_text;
    lv_obj_t* m_label;
};

class TriggerMenuItem : public MenuItem
{
    using BaseClass = MenuItem;
public:
    TriggerMenuItem(const std::function<void(void)>& callback);

    virtual void actionApply() override;

protected:
    virtual void initializeGuiImpl() override;

private:
    std::function<void(void)> m_callback;
};

template <size_t size>
class ComboBoxMenuItem : public MenuItem
{
    using BaseClass = MenuItem;
public:
    ComboBoxMenuItem(const etl::vector<const char*, size>& model, int initialIndex,
                     const std::function<bool(int)>& callback);

    virtual void actionNext() override;
    virtual void actionPrevious() override;
    virtual void actionApply() override;
    virtual void actionCancel() override;

protected:
    virtual void initializeGuiImpl() override;

private:
    void updateGui();

    etl::vector<const char*, size> m_model;
    int m_currentIndex;
    std::function<bool(int)> m_callback;

    int m_editIndex;

    lv_obj_t* m_valueLabel;
    lv_obj_t* m_leftButton;
    lv_obj_t* m_leftButtonlabel;
    lv_obj_t* m_rightButton;
    lv_obj_t* m_rightButtonlabel;
};

class SpinBoxMenuItem : public MenuItem
{
    using BaseClass = MenuItem;
public:
    SpinBoxMenuItem(int minimum, int maximum, int step, int initialValue,
                    const std::function<bool(int)>& callback);

    virtual void actionNext() override;
    virtual void actionPrevious() override;
    virtual void actionApply() override;
    virtual void actionCancel() override;

protected:
    virtual void initializeGuiImpl() override;

private:
    void updateGui();

    int m_minimum;
    int m_maximum;
    int m_step;
    int m_value;
    std::function<bool(int)> m_callback;

    int m_editValue;

    lv_obj_t* m_valueLabel;
    lv_obj_t* m_leftButton;
    lv_obj_t* m_leftButtonlabel;
    lv_obj_t* m_rightButton;
    lv_obj_t* m_rightButtonlabel;
};

// Impl

template <size_t size>
LabelMenuItem<size>::LabelMenuItem(const etl::string<size>& text)
    : BaseClass()
    , m_text(text)
{
}

template <size_t size>
void LabelMenuItem<size>::initializeGuiImpl()
{
    m_label = lv_label_create(m_object);
    lv_label_set_text(m_label, m_text.c_str());
    lv_obj_set_flex_grow(m_label, 1);
}

template <size_t size>
ComboBoxMenuItem<size>::ComboBoxMenuItem(const etl::vector<const char*, size>& model, int initialIndex, const std::function<bool(int)>& callback)
    : BaseClass()
    , m_model(model)
    , m_currentIndex(initialIndex)
    , m_callback(callback)
    , m_editIndex(initialIndex)
{

}

template <size_t size>
void ComboBoxMenuItem<size>::initializeGuiImpl()
{
    m_leftButton = lv_button_create(m_object);
    lv_obj_set_size(m_leftButton, 20, 20);

    m_leftButtonlabel = lv_label_create(m_leftButton);
    lv_label_set_text(m_leftButtonlabel, "<");
    lv_obj_align(m_leftButtonlabel, LV_ALIGN_CENTER, 0, 0);

    m_valueLabel = lv_label_create(m_object);
    lv_obj_set_flex_grow(m_valueLabel, 1);

    m_rightButton = lv_button_create(m_object);
    lv_obj_set_size(m_rightButton, 20, 20);

    m_rightButtonlabel = lv_label_create(m_rightButton);
    lv_label_set_text(m_rightButtonlabel, ">");
    lv_obj_align(m_rightButtonlabel, LV_ALIGN_CENTER, 0, 0);

    updateGui();
}

template <size_t size>
void ComboBoxMenuItem<size>::updateGui()
{
    DisplayLockGuard lock;
    lv_obj_set_state(m_leftButton, LV_STATE_DISABLED, m_editIndex == 0);
    lv_obj_set_state(m_rightButton, LV_STATE_DISABLED, m_editIndex == m_model.size() - 1);
    lv_obj_set_state(m_object, LV_STATE_EDITED, m_editIndex != m_currentIndex);
    lv_label_set_text(m_valueLabel, m_model.at(m_editIndex));
}

template <size_t size>
void ComboBoxMenuItem<size>::actionNext()
{
    if (m_editIndex < m_model.size() - 1)
    {
        m_editIndex += 1;
    }

    updateGui();
}

template <size_t size>
void ComboBoxMenuItem<size>::actionPrevious()
{
    if (m_editIndex > 0)
    {
        m_editIndex -= 1;
    }

    updateGui();
}

template <size_t size>
void ComboBoxMenuItem<size>::actionApply()
{
    if (m_callback(m_editIndex))
    {
        m_currentIndex = m_editIndex;
    }
    else
    {
        m_editIndex = m_currentIndex;
    }

    updateGui();
}

template <size_t size>
void ComboBoxMenuItem<size>::actionCancel()
{
    m_editIndex = m_currentIndex;

    updateGui();
}

#endif // MENUITEMS_HPP
