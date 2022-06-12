//------------------------------------------------------------------------------
// box.cpp
// Implementation for the Box class
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

#include "Box/box.h"

//------------------------------------------------------------------------------
// Static member initialization
EditConsole& Box::console = EditConsole::getInstance();
const Position Box::DEFAULT_POS = Position{ 0, 0 };
const Boundary Box::DEFAULT_BOUND = Boundary{ 0, 0, 0, 0 };
const int Box::DEFAULT_HEIGHT = 5;
const int Box::DEFAULT_WIDTH = 5;
const int Box::DEFAULT_HORIZ_BORDER_SIZE = 1;
const int Box::DEFAULT_VERT_BORDER_SIZE = 2;
const char Box::DEFAULT_BORDER_FILL = ' ';

//------------------------------------------------------------------------------
Box::Box() :
    absolutePos{ DEFAULT_POS },
    targetHeight{ DEFAULT_WIDTH },
    targetWidth{ DEFAULT_HEIGHT },
    actualHeight{ DEFAULT_WIDTH },
    actualWidth{ DEFAULT_HEIGHT },
    horizBorderSize{ DEFAULT_HORIZ_BORDER_SIZE },
    vertBorderSize{ DEFAULT_VERT_BORDER_SIZE },
    borderFill{ DEFAULT_BORDER_FILL },
    targetPos{ DEFAULT_POS },
    savedBound{ DEFAULT_BOUND },
    align{Alignment::LEFT | Alignment::MIDDLE},
    drawn{ false } {

}

//------------------------------------------------------------------------------
Box::Box(int width, int height) :
    absolutePos{ DEFAULT_POS },
    targetHeight{ height },
    targetWidth{ width },
    actualHeight{ height },
    actualWidth{ width },
    horizBorderSize{ DEFAULT_HORIZ_BORDER_SIZE },
    vertBorderSize{ DEFAULT_VERT_BORDER_SIZE },
    borderFill{ DEFAULT_BORDER_FILL },
    targetPos{ DEFAULT_POS },
    savedBound{ DEFAULT_BOUND },
    align{ Alignment::LEFT | Alignment::MIDDLE },
    drawn{ false } {

    // Cannot have negative width or height
    if (width < 0) {
        targetWidth = 0;
        actualWidth = 0;
    }
    if (height < 0) {
        targetHeight = 0;
        actualHeight = 0;
    }
}

//------------------------------------------------------------------------------
Reply Box::redraw() {
    // Check if Box has been drawn yet
    if (!drawn) {
        return Reply::FAILED;
    }

    return draw(targetPos, savedBound);
}

//------------------------------------------------------------------------------
Reply Box::rebuffer() {
    // Check if Box has been drawn yet
    if (!drawn) {
        return Reply::FAILED;
    }

    return buffer(targetPos, savedBound);
}

//------------------------------------------------------------------------------
void Box::setDimensions(int width, int height) {
    // Cannot have negative width or height
    if (width < 0) {
        width = 0;
    }
    if (height < 0) {
        height = 0;
    }

    targetWidth = width;
    actualWidth = width;

    targetHeight = height;
    actualHeight = height;
}

//------------------------------------------------------------------------------
void Box::setBorderSize(int size) {
    horizBorderSize = size;
    vertBorderSize = size;
}

//------------------------------------------------------------------------------
void Box::setHorizontalBorderSize(int size) {
    horizBorderSize = size;
}

//------------------------------------------------------------------------------
void Box::setVerticalBorderSize(int size) {
    vertBorderSize = size;
}

//------------------------------------------------------------------------------
void Box::setBorderFill(char fill) {
    borderFill = fill;
}

//------------------------------------------------------------------------------
void Box::setAlignment(Alignment inAlign) {
    align = inAlign;
}

//------------------------------------------------------------------------------
bool Box::posInBounds(Position pos) const {
    // Check if Box has been drawn
    if (!drawn) {
        return false;
    }

    if (pos.col < absolutePos.col || pos.col > absolutePos.col + actualWidth) {
        return false;
    }
    if (pos.row < absolutePos.row || pos.row > absolutePos.row + actualHeight) {
        return false;
    }

    return true;
}

////------------------------------------------------------------------------------
//void Box::calculateActualDimAndPos(Position pos, Boundary container) {
//    Position winDim = console.getWindowDimensions();
//    int colOffset;
//    int rowOffset;
//
//    // Save pos and container
//    targetPos = pos;
//    savedBound = container;
//
//    // Bounds check container
//    if (container.top < 0) {
//        container.top = 0;
//    }
//    if (container.left < 0) {
//        container.left = 0;
//    }
//    if (container.bottom > winDim.row) {
//        container.bottom = winDim.row;
//    }
//    if (container.right > winDim.col) {
//        container.right = winDim.col;
//    }
//
//    // Get absolute origin position of the Box
//    absolutePos.col = container.left + pos.col;
//    absolutePos.row = container.top + pos.row;
//
//    // Check if the starting position is above/behind container origin point or
//    // the console origin point
//    if (absolutePos.col < container.left) {
//        absolutePos.col = container.left;
//    }
//    if (absolutePos.row < container.top) {
//        absolutePos.row = container.top;
//    }
//
//    // Calculate offsets with overflow handling
//    if (absolutePos.col + targetWidth >= 0) {
//        // Normal calculation
//        colOffset = (absolutePos.col + targetWidth) - container.right;
//        actualWidth = targetWidth;
//    }
//    else {
//        // Prevent overflow
//        colOffset = targetWidth - container.right;
//        actualWidth = targetWidth - absolutePos.col;
//    }
//    if (absolutePos.row + targetHeight >= 0) {
//        // Normal calculation
//        rowOffset = (absolutePos.row + targetHeight) - container.bottom;
//        actualHeight = targetHeight;
//    }
//    else {
//        // Prevent overflow
//        rowOffset = targetHeight - container.bottom;
//        actualHeight = targetHeight - absolutePos.row;
//    }
//
//    // Calculate actual col and width
//    if (colOffset > 0) {
//        absolutePos.col -= colOffset;
//
//        // Check if width needs to be resized
//        if (absolutePos.col < container.left) {
//            colOffset = container.left - absolutePos.col; // Reusing colOffset
//            absolutePos.col += colOffset;
//            actualWidth -= colOffset;
//        }
//    }
//
//    // Calculate actual row and height
//    if (rowOffset > 0) {
//        absolutePos.row -= rowOffset;
//
//        // Check if height needs to be resized
//        if (absolutePos.row < container.top) {
//            rowOffset = container.top - absolutePos.row; // Reusing rowOffset
//            absolutePos.row += rowOffset;
//            actualHeight -= rowOffset;
//        }
//    }
//}

//------------------------------------------------------------------------------
void Box::calculateActualDimAndPos(Position pos, Boundary container) {
    Position winDim = console.getWindowDimensions();
    int colOffset;
    int rowOffset;

    // Save pos and container
    targetPos = pos;
    savedBound = container;

    // Bounds check container
    if (container.top < 0) {
        container.top = 0;
    }
    if (container.left < 0) {
        container.left = 0;
    }
    if (container.bottom > winDim.row) {
        container.bottom = winDim.row;
    }
    if (container.right > winDim.col) {
        container.right = winDim.col;
    }

    // Get absolute origin position of the Box
    absolutePos.col = pos.col;
    absolutePos.row = pos.row;

    // Check if the starting position is above/behind container origin point or
    // the console origin point
    if (absolutePos.col < container.left) {
        absolutePos.col = container.left;
    }
    if (absolutePos.row < container.top) {
        absolutePos.row = container.top;
    }

    // Calculate offsets with overflow handling
    if (absolutePos.col + targetWidth >= 0) {
        // Normal calculation
        colOffset = (absolutePos.col + targetWidth - 1) - container.right;
        actualWidth = targetWidth;
    }
    else {
        // Prevent overflow
        colOffset = targetWidth - container.right;
        actualWidth = targetWidth - absolutePos.col;
    }
    if (absolutePos.row + targetHeight >= 0) {
        // Normal calculation
        rowOffset = (absolutePos.row + targetHeight - 1) - container.bottom;
        actualHeight = targetHeight;
    }
    else {
        // Prevent overflow
        rowOffset = targetHeight - container.bottom;
        actualHeight = targetHeight - absolutePos.row;
    }

    // Calculate actual col and width
    if (colOffset > 0) {
        absolutePos.col -= colOffset;

        // Check if width needs to be resized
        if (absolutePos.col < container.left) {
            colOffset = container.left - absolutePos.col; // Reusing colOffset
            absolutePos.col += colOffset;
            actualWidth -= colOffset;
        }
    }

    // Calculate actual row and height
    if (rowOffset > 0) {
        absolutePos.row -= rowOffset;

        // Check if height needs to be resized
        if (absolutePos.row < container.top) {
            rowOffset = container.top - absolutePos.row; // Reusing rowOffset
            absolutePos.row += rowOffset;
            actualHeight -= rowOffset;
        }
    }
}

//------------------------------------------------------------------------------
void Box::printBase(Position pos, Boundary container) {
    calculateActualDimAndPos(pos, container);
    std::string borderRow(actualWidth, borderFill);
    std::string internalRow(actualWidth, ' ');

    // Add borders to internalRows
    for (int i = 0; i < vertBorderSize && i < actualWidth; ++i) {
        internalRow[i] = borderFill;
        internalRow[actualWidth - i - 1] = borderFill;
    }

    // Backup current cursor position
    Position backupPos = console.getCursorPosition();

    // Draw Box base to console
    for (int i = 0; i < actualHeight; ++i) {
        console.setCursorPosition(Position{ absolutePos.col, 
                absolutePos.row + i });
        if (i + 1 <= horizBorderSize || actualHeight - i <= horizBorderSize) {
            std::cout << borderRow;
        }
        else {
            std::cout << internalRow;
        }
    }

    // Resore cursor potition
    console.setCursorPosition(backupPos);
}

//------------------------------------------------------------------------------
void Box::bufferBase(Position pos, Boundary container) {
    calculateActualDimAndPos(pos, container);
    std::string borderRow(actualWidth, borderFill);
    std::string internalRow(actualWidth, ' ');

    // Add borders to internalRows
    for (int i = 0; i < vertBorderSize && i < actualWidth; ++i) {
        internalRow[i] = borderFill;
        internalRow[actualWidth - i - 1] = borderFill;
    }

    // Draw Box base to console
    for (int i = 0; i < actualHeight; ++i) {
        if (i + 1 <= horizBorderSize || actualHeight - i <= horizBorderSize) {
            console.writeToBuffer(Position{ absolutePos.col, 
                    absolutePos.row + i }, borderRow.c_str());
        }
        else {
            console.writeToBuffer(Position{ absolutePos.col,
                    absolutePos.row + i }, internalRow.c_str());
        }
    }
}