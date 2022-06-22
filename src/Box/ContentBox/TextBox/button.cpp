//------------------------------------------------------------------------------
// button.cpp
// Implementation for the Button class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Button is a type of TextBox that performs some user-defined
//     execution when clicked on. 
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

#include "Box/ContentBox/TextBox/button.h"

namespace conu {

Reply Button::DEFAULT_CLICK_HANDLER(Box& self) {
    return Reply::CONTINUE;
}

//--------------------------------------------------------------------------
Button::Button(std::string text) :
    TextBox(text),
    clickHandler{ DEFAULT_CLICK_HANDLER } {

}

//--------------------------------------------------------------------------
Button::Button(int width, int height, std::string text) :
    TextBox(width, height, text),
    clickHandler{ DEFAULT_CLICK_HANDLER } {

}

//--------------------------------------------------------------------------
Reply Button::interact(inputEvent::MouseEvent action) {
    // Check if Box has been drawn
    if (!drawn) {
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
void Button::setAction(std::function<Reply(Box&)> action) {
    clickHandler = action;
}

}