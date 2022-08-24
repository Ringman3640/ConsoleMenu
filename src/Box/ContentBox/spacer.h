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

private:
    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) override;

};

}
