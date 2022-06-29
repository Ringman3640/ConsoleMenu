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
// Class Functionality:
//     - Defines the minimum interface for all Box objects.
//     - Draw the Box to the program console.
//     - Get and set the dimensions of the Box object.
//     - Set the size of the horizontal and vertical border invididually or 
//       concurrently. 
//     - Set the character that will be used to print the console border.
//     - Set the horizontal and vertical alignment of the Box contents. 
// 
// Dependencies: EditConsole class and Flag enumerators.
//------------------------------------------------------------------------------

#pragma once

#define MAXIMUM (std::numeric_limits<int>::max)()

#include <iostream>
#include <string>
#include <limits>
#include "ConsoleEditor/consoleeditor.h"
#include "Flag/flag.h"

namespace conu {

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
    // Draw the Box to the output console given an origin column and row, and
    // a constraining rectangle that represents the container boundaries that
    // the Box is within.
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
    // Redraw the Box given the same conditions as the previous draw() or 
    // buffer() call.
    virtual Reply redraw();

    //--------------------------------------------------------------------------
    // Rebuffer the Box to EditConsole's write buffer given the same conditions
    // as teh previous draw() or buffer() call.
    virtual Reply rebuffer();

    //--------------------------------------------------------------------------
    // Set the target width and height of the Box.
    void setDimensions(int width, int height);

    //--------------------------------------------------------------------------
    // Set the size in character units of the horizontal and vertical border 
    // sides.
    void setBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the size in character units of the horizontal border sides.
    void setHorizontalBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the size in character units of the vertical border sides.
    void setVerticalBorderSize(int size);

    //--------------------------------------------------------------------------
    // Set the fill character for the horizontal and vertical borders.
    void setBorderFill(char fill);

    //--------------------------------------------------------------------------
    // Set the individual fill characters for all border sides.
    void setBorderFill(const BorderFill& fill);

    //--------------------------------------------------------------------------
    // Set the alignment of the Box contents.
    void setAlignment(Align inAlign);

    //--------------------------------------------------------------------------
    // Check if the provided coordinate position is within the bounds of the
    // Box object. Returns false if the Box has not been drawn yet.
    bool posInBounds(Position pos) const;

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

    //--------------------------------------------------------------------------
    // Calculate the actual dimentions and position of the Box. Returns the
    // calculated absolute origin position of the ContentBox (relative to the
    // origin of the console screen).
    virtual void calculateActualDimAndPos(Position pos, Boundary container);

    //--------------------------------------------------------------------------
    // Print the base of the Box, including the Box borders and clearing the 
    // inside of the Box. Returns the calculated absolute origin position of the
    // ContentBox (realative to the origin of the console screen).
    virtual void printBase(Position pos, Boundary container);

    //--------------------------------------------------------------------------
    // Buffer the base of the Box to EditConsole's write buffer. Returns the
    // calculated absolute origin position of the Box (realative to the origin 
    // of the console screen).
    virtual void bufferBase(Position pos, Boundary container);

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

}