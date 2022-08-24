//------------------------------------------------------------------------------
// scrollingtextbox.h
// Interface for the ScrollingTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A ScrollingTextBox is a type of TextBox that allows the user-
//     defined text to be scrolled within the Box if there is not enought space
//     to display the entire text message at once. 
//
// Class Functionality:
//     - Inherits all methods and functionality of the TextBox class.
//     - Draw the ScrollingTextBox to the console window.
//     - Set the text of the ScrollingTextBox that will be printed.
//     - Scroll through the contents of the TextBox given vertical mouse scroll
//       inputs.
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#pragma once

#include "Box/ContentBox/TextBox/textbox.h"

namespace conu {

//------------------------------------------------------------------------------
class ScrollingTextBox : public TextBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    ScrollingTextBox();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    ScrollingTextBox(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Interact with the ScrollingTextBox given a MouseEvent. Any MouseEvent
    // that indicates a scroll will change the scroll position of the 
    // ScrollingTextBox's contents.
    // If the Box has not been drawn yet, FAILED is returned.
    // If the MouseEvent is out-of-bounds or not a scroll, IGNORED is returned.
    // If the ScrollingTextBox is successfully scrolled, REFRESH is returned. 
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this ScrollingTextBox object and return a pointer 
    // to that copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new ScrollingTextBox object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific ScrollingTextBox class. Returns 
    // "ScrollingTextBox".
    virtual std::string getClassName() const override;

private:
    int scrollPos;

    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) override;

};

}
