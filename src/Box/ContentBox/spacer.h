//------------------------------------------------------------------------------
// spacer.h
// Interface for the Spacer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Spacer is a type of ContentBox that does not hold or print
//     any internal content. Spacer objects act as padding on the console
//     screen for other Box objects. Does not produce any output or action
//     upon interaction.
//
// Class Functionality:
//     - Inherits all methods and functionality of the ContentBox class.
//     - Implements the draw() and interact() ContentBox methods.
//     - Draw the Spacer to the console window.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#pragma once

#include "Box/ContentBox/contentbox.h"

namespace conu {

//------------------------------------------------------------------------------
class Spacer : public ContentBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    Spacer();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    Spacer(int width, int height);

    //--------------------------------------------------------------------------
    // Draw the Spacer to the output console given an origin column and row, and
    // a constraining rectangle that represents the container boundaries that
    // the Spacer is within. Returns a CONTINUE Reply.
    virtual Reply draw(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Buffer the Box to EditConsole's write buffer given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the Box is within. 
    virtual Reply buffer(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event. Returns an IGNORED Reply.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this Spacer object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new Spacer object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific Spacer class. Returns "Spacer".
    virtual std::string getClassName() const override;

};

}