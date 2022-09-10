//------------------------------------------------------------------------------
// exitbutton.cpp
// Implementation for the ExitButton class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An ExitButton is a Button that will return an EXIT Reply when
//     interacted with a left click. Before returning EXIT, a user-defined
//     execution is performed if provided.
// 
// Dependencies: Button class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/Button/exitbutton.h"

namespace conu {

//------------------------------------------------------------------------------
ExitButton::ExitButton(std::string text) :
    Button{ text } {

}

//------------------------------------------------------------------------------
ExitButton::ExitButton(int width, int height, std::string text) :
    Button{ width, height, text } {

}

//------------------------------------------------------------------------------
Reply ExitButton::interact(inputEvent::MouseEvent action) {
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
    if (clickHandler) {
        clickHandler(*this);
    }
    return Reply::EXIT;
}

//------------------------------------------------------------------------------
Box* ExitButton::copyBox() const {
    return new ExitButton(*this);
}

//------------------------------------------------------------------------------
Box* ExitButton::createBox() const {
    return new ExitButton();
}

//------------------------------------------------------------------------------
std::string ExitButton::getClassName() const {
    return std::string("ExitButton");
}

}