#ifndef MENU_HPP
#define MENU_HPP

#include "menuitems.hpp"
#include "guiaction.hpp"

#include "bsp/esp-bsp.h"

#include <vector>
#include <memory>
#include <functional>

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

    void invokeAction(GuiAction action);

private:
    lv_obj_t* createLabel(lv_obj_t* parent, const MenuLine& line);

    lv_obj_t* m_menuRootObject;
    std::vector<MenuLine> m_menuLines;
    std::vector<lv_obj_t*> m_lineTitleLabels;
    std::vector<int32_t> m_rowDescription;
    std::vector<int32_t> m_columnDescription;

    int m_currentIndex;
};

#endif // MENU_HPP