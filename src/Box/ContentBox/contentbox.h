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
    // Draw the ContentBox to the output console given an origin column and row,
    // and a constraining rectangle that represents the container boundaries
    // that the ContainerBox is within.
    virtual Reply draw(Position pos, Boundary container) = 0;

    //--------------------------------------------------------------------------
    // Buffer the Box to EditConsole's write buffer given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the Box is within. 
    virtual Reply buffer(Position pos, Boundary container) = 0;

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

};

}