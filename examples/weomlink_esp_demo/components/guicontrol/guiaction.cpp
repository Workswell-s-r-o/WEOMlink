#include "guiaction.hpp"

#include <cassert>

const char* guiActionToString(GuiAction action)
{
    switch (action)
    {
        case GuiAction::NEXT:
            return "NEXT";
        case GuiAction::PREVIOUS:
            return "PREVIOUS";
        case GuiAction::UP:
            return "UP";
        case GuiAction::DOWN:
            return "DOWN";
        case GuiAction::APPLY:
            return "APPLY";
        case GuiAction::CANCEL:
            return "CANCEL";
    }
    assert(false);
    return "";
}