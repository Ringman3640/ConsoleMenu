//------------------------------------------------------------------------------
// exitbutton.h
// Interface for the ExitButton class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An ExitButton is a Button that will return an EXIT Reply when
//     interacted with a left click. Before returning EXIT, a user-defined
//     execution is performed if provided.
// 
// Dependencies: Button class.
//------------------------------------------------------------------------------

#pragma once

#include "Box/ContentBox/TextBox/Button/button.h"

namespace conu {

class ExitButton : public Button {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    ExitButton(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructor
    ExitButton(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Execute an action given a mouse click. Returns IGNORED if the mouse
    // event is not a left click. Otherwise returns EXIT.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this ExitButton object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new ExitButton object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific ExitButton class. Returns "ExitButton".
    virtual std::string getClassName() const override;

};

}