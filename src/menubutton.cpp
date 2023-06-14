//------------------------------------------------------------------------------
// menubutton.cpp
// Implementation for the MenuButton class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A MenuButton is a Button that enters a specified Menu when
//     interacted with a mouse click event. Before entering the Menu, a user-
//     defined execution is perfomed if provided. 
//
// Dependencies: Button class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/Button/menubutton.h"

namespace conu {

//------------------------------------------------------------------------------
MenuButton::MenuButton(std::string text) :
    Button{ text },
    entryMenu{ nullptr } {

}

//------------------------------------------------------------------------------
MenuButton::MenuButton(int width, int height, std::string text) :
    Button{ width, height, text },
    entryMenu{ nullptr } {

}

//------------------------------------------------------------------------------
MenuButton::MenuButton(int width, int height, Menu& entryMenu,
        std::string text) :
    Button{ width, height, text },
    entryMenu{ &entryMenu } {

}

//------------------------------------------------------------------------------
Reply MenuButton::interact(inputEvent::MouseEvent action) {
    // Check if Box has been drawn
    if (!drawn) {
        return Reply::FAILED;
    }
    if (entryMenu == nullptr) {
        return Reply::FAILED;
    }

    // Check if action is in-bounds
    if (!posInBounds(action.mousePosition)) {
        return Reply::IGNORED;
    }

    // Check if action is a left click
    if (!action.leftClick || action.eventFlag
        != inputEvent::Mouse::CLICKED) {
        return Reply::IGNORED;
    }

    // Execute click handler (if present) and enter Menu
    if (clickHandler) {
        clickHandler(*this);
    }
    return entryMenu->enter();
}

//------------------------------------------------------------------------------
Box* MenuButton::copyBox() const {
    return new MenuButton(*this);
}

//------------------------------------------------------------------------------
Box* MenuButton::createBox() const {
    return new MenuButton();
}

//------------------------------------------------------------------------------
std::string MenuButton::getClassName() const {
    return std::string("MenuButton");
}

//------------------------------------------------------------------------------
void MenuButton::setEntryMenu(Menu& entryMenu) {
    this->entryMenu = &entryMenu;
}

}