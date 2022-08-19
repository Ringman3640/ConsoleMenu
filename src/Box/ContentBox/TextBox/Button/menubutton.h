//------------------------------------------------------------------------------
// menubutton.h
// Interface for the MenuButton class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A MenuButton is a Button that enters a specified Menu when
//     interacted with a mouse click event. Before entering the Menu, a user-
//     defined execution is perfomed if provided. 
// 
// Dependencies: Button class.
//------------------------------------------------------------------------------

#pragma once

#include "Box/ContentBox/TextBox/Button/button.h"
#include "Menu/menu.h"

namespace conu {

class MenuButton : public Button {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    MenuButton(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructors
    MenuButton(int width, int height, std::string text = std::string());
    MenuButton(int width, int height, Menu& entryMenu, 
            std::string text = std::string());

    //--------------------------------------------------------------------------
    // Execute an action given a mouse click. Returns IGNORED if the mouse
    // event is not a left click. Otherwise returns a user-specified Reply.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this MenuButton object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new MenuButton object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific MenuButton class. Returns "Button".
    virtual std::string getClassName() const override;

private:
    //--------------------------------------------------------------------------
    // Pointer to the Menu that is to be entered upon interact
    Menu* entryMenu;

};

}