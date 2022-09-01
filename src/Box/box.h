//------------------------------------------------------------------------------
// box.h
// Interface for the Box class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Box is an object that displays content to the console screen.
//     Each box has a specified height and width and is printed to the screen 
//     defined by the coordinate of its top left corner relative to the origin 
//     of the console screen (top corner of the screen or position 0, 0). All
//     Boxes have an internal border that restrict the position of its internal
//     contents. These borders can be invisible or printed to the console
//     screen with a specified character fill. The contents of the Box are
//     aligned within the Box given specified horizontal and vertical alignment
//     flags.
// 
// Dependencies: EditConsole class and Flag enumerators.
//------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "ConsoleEditor/consoleeditor.h"
#include "Flag/flag.h"

namespace conu {

const int MAXIMUM = (std::numeric_limits<int>::max)();

//------------------------------------------------------------------------------
// BorderFill struct
// Contains information about the border fill characters for each side of
// the box object.
struct BorderFill {
    char left;
    char top;
    char right;
    char bottom;
};

//------------------------------------------------------------------------------
class Box {
public:

    //--------------------------------------------------------------------------
    // Default constructor
    Box();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    Box(int width, int height);

    //--------------------------------------------------------------------------
    // Virtual destructor
    virtual ~Box();

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event. 
    virtual Reply interact(inputEvent::MouseEvent action) = 0;

    //--------------------------------------------------------------------------
    // Create a deep copy of this Box object and return a pointer to that copy.
    virtual Box* copyBox() const = 0;

    //--------------------------------------------------------------------------
    // Create a new Box object and return a pointer to that object.
    virtual Box* createBox() const = 0;

    //--------------------------------------------------------------------------
    // Get the name of the specific Box class.
    virtual std::string getClassName() const = 0;

    //--------------------------------------------------------------------------
    // Get the target height of the Box.
    virtual int getHeight() const = 0;

    //--------------------------------------------------------------------------
    // Get the target width of the Box.
    virtual int getWidth() const = 0;

    //--------------------------------------------------------------------------
    // Get the actual position of the Box when printed. 
    // If the Box was not printed before, returns Position { -1, -1 }.
    virtual Position getPosition() const;

    //--------------------------------------------------------------------------
    // Draw the Box to the output console given an origin column and row, and
    // a constraining rectangle that represents the container boundaries that
    // the Box is within.
    virtual Reply draw(Position pos, Boundary container);

    //--------------------------------------------------------------------------
    // Buffer the Box to EditConsole's write buffer given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the Box is within. 
    virtual Reply buffer(Position pos, Boundary container);

    //--------------------------------------------------------------------------
    // Redraw the Box given the same conditions as the previous draw() or 
    // buffer() call.
    // Requires that the Box have been printed through draw() or buffer()
    // previously. Otherwise, returns FAILED.
    virtual Reply redraw();

    //--------------------------------------------------------------------------
    // Rebuffer the Box to EditConsole's write buffer given the same conditions
    // as the previous draw() or buffer() call.
    // Requires that the Box have been printed through draw() or buffer()
    // previously. Otherwise, returns FAILED.
    virtual Reply rebuffer();

    //--------------------------------------------------------------------------
    // Set the target width and height of the Box.
    virtual void setDimensions(int width, int height);

    //--------------------------------------------------------------------------
    // Set the size in character units of the horizontal and vertical border 
    // sides.
    virtual void setBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the size in character units of the horizontal border sides.
    virtual void setHorizontalBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the size in character units of the vertical border sides.
    virtual void setVerticalBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the fill character for the horizontal and vertical borders.
    virtual void setBorderFill(char fill);

    //--------------------------------------------------------------------------
    // Set the individual fill characters for all border sides.
    virtual void setBorderFill(const BorderFill& fill);

    //--------------------------------------------------------------------------
    // Set the alignment of the Box contents.
    virtual void setAlignment(Align inAlign);

    //--------------------------------------------------------------------------
    // Set whether the Box's base background is transparent.
    virtual void backgroundTransparent(bool transparent);

    //--------------------------------------------------------------------------
    // Check if the provided coordinate position is within the bounds of the
    // Box object. Returns false if the Box has not been drawn yet.
    virtual bool posInBounds(Position pos) const;

protected:
    //--------------------------------------------------------------------------
    // Box data members

    // Static console object
    static ConsoleEditor& console;

    // Box dimensions and position information
    Position absolutePos;
    int targetHeight;
    int targetWidth;
    int actualHeight;
    int actualWidth;

    // Box border information
    int horizBorderSize;
    int vertBorderSize;
    BorderFill borderFill;

    // Redraw method utilization
    Position targetPos;
    Boundary savedBound;

    // Drawing information
    Align alignment;
    bool drawn;
    bool transparent;

    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container, 
            bool drawMode) = 0;

    //--------------------------------------------------------------------------
    // Print a line of text to the console's screen or buffer (indicated by
    // the drawMode parameter).
    // Helper function for printProtocol.
    void printLine(const Position& pos, const char text[], bool drawMode);

    //--------------------------------------------------------------------------
    // Calculate the actual dimentions and position of the Box. Returns the
    // calculated absolute origin position of the ContentBox (relative to the
    // origin of the console screen).
    virtual void calculateActualDimAndPos(Position pos, Boundary container);

    //--------------------------------------------------------------------------
    // Print the base of the Box, including the Box borders and clearing the 
    // inside of the Box.
    // Automatically calls calculateActualDimAndPos().
    // Indicate if the base should be drawn or buffered with the drawMode
    // parameter.
    virtual void printBase(Position pos, Boundary container, bool drawMode);

private:
    //--------------------------------------------------------------------------
    // Static const members
    static const Position DEFAULT_POS;
    static const Boundary DEFAULT_BOUND;
    static const int DEFAULT_HEIGHT;
    static const int DEFAULT_WIDTH;
    static const int DEFAULT_HORIZ_BORDER_SIZE;
    static const int DEFAULT_VERT_BORDER_SIZE;
    static const BorderFill DEFAULT_BORDER_FILL;

};

//------------------------------------------------------------------------------
// Inline printLine() definition.
inline void Box::printLine(const Position& pos, const char text[], 
        bool useDrawing) {
    useDrawing ? console.writeToScreen(pos, text) 
            : console.writeToBuffer(pos, text);
}

}
