//------------------------------------------------------------------------------
// contentbox.h
// Interface for the ContentBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A ContentBox is a type of Box object that specifically produces
//     a visual and optionally interactable element to the console screen. All
//     ContentBox objects are contained Boxes with a single executable draw()
//     and interact() method.
//
// Class Functionality:
//     - Inherits all methods and functionality of the Box class.
//     - Implements the getHeight() and getWidth() Box methods.
//     - Get the height and width dimensions of the ContentBox.
// 
// Dependencies: Box class.
//------------------------------------------------------------------------------

#pragma once

#include "Box/box.h"

namespace conu {

//------------------------------------------------------------------------------
class ContentBox : public Box {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    ContentBox();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    ContentBox(int width, int height);

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event.
    virtual Reply interact(inputEvent::MouseEvent action) = 0;

    //--------------------------------------------------------------------------
    // Create a deep copy of this ContentBox object and return a pointer to that
    // copy.
    virtual Box* copyBox() const = 0;

    //--------------------------------------------------------------------------
    // Create a new ContentBox object and return a pointer to that object.
    virtual Box* createBox() const = 0;

    //--------------------------------------------------------------------------
    // Get the name of the specific ContentBox class.
    virtual std::string getClassName() const = 0;

    //--------------------------------------------------------------------------
    // Get the target height of the Box.
    virtual int getHeight() const override;

    //--------------------------------------------------------------------------
    // Get the target width of the Box.
    virtual int getWidth() const override;

private:
    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) = 0;

};

}
