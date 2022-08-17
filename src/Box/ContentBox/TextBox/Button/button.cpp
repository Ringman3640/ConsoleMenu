//------------------------------------------------------------------------------
// button.cpp
// Implementation for the Button class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Button is a type of TextBox that performs some user-defined
//     execution when interacted with a mouse click event.
//
// Class Functionality:
//     - Inherits all methods and functionality of the TextBox class.
//     - Draw the Button to the console window.
//     - Set the text of the Button that will be printed.
//     - Interact with the Button with a left click, which performs a 
//       user-defined execution.
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/Button/button.h"

namespace conu {

//--------------------------------------------------------------------------
Button::Button(std::string text) :
    TextBox(text),
    clickHandler{ nullptr } {

}

//--------------------------------------------------------------------------
Button::Button(int width, int height, std::string text) :
    TextBox(width, height, text),
    clickHandler{ nullptr } {

}

//--------------------------------------------------------------------------
Reply Button::interact(inputEvent::MouseEvent action) {
    // Check if Box has been drawn
    if (!drawn) {
        return Reply::FAILED;
    }
    if (clickHandler == nullptr) {
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
}

}