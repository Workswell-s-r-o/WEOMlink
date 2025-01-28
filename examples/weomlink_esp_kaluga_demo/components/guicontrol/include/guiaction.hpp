#ifndef GUIACTION_HPP
#define GUIACTION_HPP


enum class GuiAction
{
    NEXT,
    PREVIOUS,
    UP,
    DOWN,
    APPLY,
    CANCEL
};

const char* guiActionToString(GuiAction action);

#endif // GUIACTION_HPP