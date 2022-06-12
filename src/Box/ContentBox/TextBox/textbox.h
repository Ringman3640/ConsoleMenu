//------------------------------------------------------------------------------
// textbox.h
// Interface for the TextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A TextBox is a type of ContentBox that contains user-defined
//     text that is printed within the dimensions of the TextBox to the console
//     window. The text content is aligned within the TextBox depending on the
//     alignment flags specified. Does not produce any output or action upon
//     interaction.
//
// Class Functionality:
//     - Inherits all methods and functionality of the ContentBox class.
//     - Implements the draw() and interact() ContentBox methods.
//     - Draw the TextBox object to the console window.
//     - Set the text of the TextBox that will be printed.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include "Box/ContentBox/contentbox.h"

//------------------------------------------------------------------------------
class TextBox : public ContentBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    TextBox(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructor
    TextBox(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Draw the TextBox to the output console given an origin column and row,
    // and a constraining rectangle that represents the container boundaries
    // that the TextBox is within. Returns a CONTINUE Reply.
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
    // Create a deep copy of this TextBox object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new TextBox object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific TextBox class. Returns "TextBox".
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Set the content text of the TextBox.
    void setText(std::string text);

protected:
    std::string text;
    std::vector<std::string> lines;

    //--------------------------------------------------------------------------
    // Split the text content into individual lines to fit that TextBox's 
    // actual width
    void splitText();

    //--------------------------------------------------------------------------
    // Apply horizontal spacing alignment to all strings in lines.
    void applyHorizontalAlignment();

};