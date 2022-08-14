//------------------------------------------------------------------------------
// graphic.h
// Interface for the Graphic class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Graphic is a ContentBox that manages a two-dimensional char
//     array. This array is refered to as the canvas of the Graphic, and is
//     modifiable by the user through the Graphic's [] operator and at() method.
//     Graphics are responsible for correctly displaying its canvas given
//     changes to the dimensions and position of the Graphic at run-time. The
//     Graphic's Alignment selection will modify how the canvas is displayed
//     if the visible area is smaller than the size of the canvas.
//
// Class Functionality:
//     - Inherits all methods and functionality of the ContentBox class.
//     - Access and modify the Graphic's canvas by character or by row line. 
//     - Formats canvas positioning if visible area is reduced depending on
//       Alignment selection.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#pragma once

#include <vector>
#include <stdexcept>
#include "Box/ContentBox/contentbox.h"

namespace conu {

class GraphicLine;

//------------------------------------------------------------------------------
class Graphic : public ContentBox {
    friend class GraphicLine;

public:
    //--------------------------------------------------------------------------
    // Default constructor
    Graphic();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    Graphic(int width, int height);

    //--------------------------------------------------------------------------
    // Draw the Graphic to the output console given an origin column and row,
    // and a constraining rectangle that represents the container boundaries
    // that the Graphic is within. Returns a CONTINUE Reply.
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
    // Create a deep copy of this Graphic object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new Graphic object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific Graphic class. Returns "Graphic".
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Get a GraphicLine from the Graphic's canvas.
    // Does not bounds check
    GraphicLine operator [] (int idx);

    //--------------------------------------------------------------------------
    // Get a GraphicLine from the Graphic's canvas.
    // Throws out_of_range exception if index is outisde the canvas range.
    GraphicLine at(int idx);

private:
    static const char DEFAULT_CANVAS_FILL;
    std::vector<std::vector<char>> canvas;

    //--------------------------------------------------------------------------
    // Resize the canvas to fit the Graphic dimensions
    void updateCanvasSize();

    //--------------------------------------------------------------------------
    // Get the horizontal offset of the canvas given that the visible area of
    //     the Graphic is smaller than the canvas size.
    // Helper method for draw() and buffer().
    int getHorizontalOffset();

    //--------------------------------------------------------------------------
    // Get the vertical offset of the canvas given that the visible area of
    //     the Graphic is smaller than the canvas size.
    // Helper method for draw() and buffer().
    int getVerticalOffset();

};

//------------------------------------------------------------------------------
// GraphicLine class
// Provides modification access to a specific char array in a Graphic instance's
//     canvas. 
// Helper class for Graphic
class GraphicLine {
    friend class Graphic;

public:
    //--------------------------------------------------------------------------
    // Assign the GraphicLine with a string input starting at index 0. 
    // Cuts-off the string if it is longer than the GraphicLine.
    void operator = (std::string lineText);

    //--------------------------------------------------------------------------
    // Get a reference to a character in the GraphicLine.
    // Does not bounds check.
    char& operator [] (int idx);

    //--------------------------------------------------------------------------
    // Get a reference to a character in the GraphicLine.
    // Throws out_of_range exception if index is outisde the line range.
    char& at(int idx);

private:
    std::vector<char>* canvasLine;

    //--------------------------------------------------------------------------
    // Private default constructor
    GraphicLine(std::vector<char>& line);
};

}