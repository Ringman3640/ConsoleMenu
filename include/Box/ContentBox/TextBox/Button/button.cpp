//------------------------------------------------------------------------------
// button.cpp
// Implementation for the Button class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Button is a type of TextBox that performs some user-defined
//     execution when interacted with a mouse click event.
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/Button/button.h"

namespace conu {

//--------------------------------------------------------------------------
Button::Button(std::string text) :
    TextBox(text),
    clickHandler{ nullptr },
    inputClickHandler{ nullptr }, 
    passInput{ false } {

}

//--------------------------------------------------------------------------
Button::Button(int width, int height, std::string text) :
    TextBox(width, height, text),
    clickHandler{ nullptr },
    inputClickHandler{ nullptr },
    passInput{ false } {

}

//--------------------------------------------------------------------------
Reply Button::interact(inputEvent::MouseEvent action) {
    // Check if Box has been drawn
    if (!drawn) {
        return Reply::FAILED;
    }
    if (!passInput && clickHandler == nullptr) {
        return Reply::FAILED;
    }
    if (passInput && inputClickHandler == nullptr) {
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

    // Execute click handler and return reply
    if (passInput) {
        return inputClickHandler(*this, action);
    }
    return clickHandler(*this);
}

//--------------------------------------------------------------------------
Box* Button::copyBox() const {
    return new Button(*this);
}

//--------------------------------------------------------------------------
Box* Button::createBox() const {
    return new Button();
}

//--------------------------------------------------------------------------
std::string Button::getClassName() const {
    return std::string("Button");
}

//--------------------------------------------------------------------------
void Button::setAction(std::function<Reply(Button&)> action) {
    clickHandler = action;
    passInput = false;
}

//--------------------------------------------------------------------------
void Button::setAction(std::function<Reply(Button&, 
        inputEvent::MouseEvent)> action) {
    inputClickHandler = action;
    passInput = true;
}

}