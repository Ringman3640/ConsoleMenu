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
    // Draw the ScrollingTextBox to the output console given an origin column
    // and row, and a constraining rectangle that represents the container
    // boundaries that the TextBox is within. Returns a CONTINUE Reply.
    virtual Reply draw(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Buffer the Box to EditConsole's write buffer given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the Box is within. 
    virtual Reply buffer(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event. Returns a CONTINUE Reply.
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
    // Draw the ScrollingTextBox given calculated width, height, and position.
    // Helper fuction for draw() and interact().
    void drawScrollTBox();

    //--------------------------------------------------------------------------
    // Buffer the ScrollingTextBox given calculated width, height, and position
    // to EditConsole's write buffer. Helper fuction for draw() and interact().
    void bufferScrollTBox();

};

}