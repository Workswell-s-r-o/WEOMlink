#ifndef MENU_HPP
#define MENU_HPP

#include "menuitems.hpp"

#include "bsp/esp-bsp.h"

#include <vector>
#include <memory>
#include <functional>
#include <chrono>


struct MenuLine
{
    std::string title;
    std::shared_ptr<MenuItem> item;
};

class Menu
{
public:
    Menu(const std::vector<MenuLine>& menuLines);
    ~Menu();

private:
    lv_obj_t* createLabel(lv_obj_t* parent, const MenuLine& line);
    void next();
    void previous();
    void apply();
    void cancel();

    lv_obj_t* m_menuRootObject;
    lv_obj_t* m_panelRoot;

    std::vector<MenuLine> m_menuLines;
    int m_currentIndex;

    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    TimePoint m_lastScrollTimePoint;
    static constexpr std::chrono::milliseconds SCROLL_DELAY_TIME{200};
};

#endif // MENU_HPP