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
// Dependencies: EditConsole class and Flag enumerators.
//------------------------------------------------------------------------------

#include "Box/box.h"

namespace conu {

//------------------------------------------------------------------------------
// Static member initialization
ConsoleEditor& Box::console = ConsoleEditor::getInstance();
const Position Box::DEFAULT_POS = Position{ 0, 0 };
const Boundary Box::DEFAULT_BOUND = Boundary{ 0, 0, 0, 0 };
const int Box::DEFAULT_HEIGHT = 5;
const int Box::DEFAULT_WIDTH = 5;
const int Box::DEFAULT_HORIZ_BORDER_SIZE = 1;
const int Box::DEFAULT_VERT_BORDER_SIZE = 2;
const BorderFill Box::DEFAULT_BORDER_FILL = BorderFill{ ' ', ' ', ' ', ' ' };

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
    alignment{ Align::LEFT | Align::MIDDLE },
    drawn{ false },
    transparent{ false } {

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
    alignment{ Align::LEFT | Align::MIDDLE },
    drawn{ false },
    transparent{ false } {

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
Box::~Box() {

}

//------------------------------------------------------------------------------
Reply Box::draw(Position pos, Boundary container) {
    return printProtocol(pos, container, true);
}

//------------------------------------------------------------------------------
Reply Box::buffer(Position pos, Boundary container) {
    return printProtocol(pos, container, false);
}

//------------------------------------------------------------------------------
Reply Box::redraw() {
    if (!drawn) {
        return Reply::FAILED;
    }

    return printProtocol(targetPos, savedBound, true);
}

//------------------------------------------------------------------------------
Reply Box::rebuffer() {
    if (!drawn) {
        return Reply::FAILED;
    }

    return printProtocol(targetPos, savedBound, false);
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
    borderFill.left = fill;
    borderFill.top = fill;
    borderFill.right = fill;
    borderFill.bottom = fill;
}

//------------------------------------------------------------------------------
void Box::setBorderFill(const BorderFill& fill) {
    borderFill.left = fill.left;
    borderFill.top = fill.top;
    borderFill.right = fill.right;
    borderFill.bottom = fill.bottom;
}

//------------------------------------------------------------------------------
void Box::setAlignment(Align inAlign) {
    alignment = inAlign;
}

//------------------------------------------------------------------------------
void Box::backgroundTransparent(bool transparent) {
    this->transparent = transparent;
}

//------------------------------------------------------------------------------
bool Box::posInBounds(Position pos) const {
    // Check if Box has been drawn
    if (!drawn) {
        return false;
    }

    if (pos.col < absolutePos.col || pos.col > absolutePos.col 
            + actualWidth - 1) {
        return false;
    }
    if (pos.row < absolutePos.row || pos.row > absolutePos.row 
            + actualHeight - 1) {
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
    if (container.bottom > --winDim.row) {
        container.bottom = winDim.row;
    }
    if (container.right > --winDim.col) {
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

    // Check actual width and height for negative
    if (actualWidth < 0) {
        actualWidth = 0;
    }
    if (actualHeight < 0) {
        actualHeight = 0;
    }
}

//------------------------------------------------------------------------------
void Box::printBase(Position pos, Boundary container, bool drawMode) {
    calculateActualDimAndPos(pos, container);
    std::string topBorderRow(actualWidth, borderFill.top);
    std::string bottomBorderRow(actualWidth, borderFill.bottom);
    std::string internalRow(actualWidth, ' ');
    Position currPos = absolutePos;

    // Add vertical borders
    for (int i = 0; i < vertBorderSize && i < actualWidth; ++i) {
        topBorderRow[i] = borderFill.left;
        bottomBorderRow[i] = borderFill.left;
        internalRow[i] = borderFill.left;

        topBorderRow[actualWidth - i - 1] = borderFill.right;
        bottomBorderRow[actualWidth - i - 1] = borderFill.right;
        internalRow[actualWidth - i - 1] = borderFill.right;
    }

    // Only print border if transparent
    if (transparent) {
        if (horizBorderSize == 0 && vertBorderSize == 0) {
            return;
        }

        for (int i = 0; i < actualHeight; ++i) {
            if (i + 1 <= horizBorderSize) {
                printLine(currPos, topBorderRow.c_str(), drawMode);
            }
            else if (actualHeight - i <= horizBorderSize) {
                printLine(currPos, bottomBorderRow.c_str(), drawMode);
            }
            else {
                // Print vertical borders
                int maxCol = currPos.col + actualWidth;
                for (int j = 0; j < vertBorderSize; ++j) {
                    char temp[2] = { borderFill.left, '\0' };
                    printLine(Position{ currPos.col + i, currPos.row }, temp,
                        drawMode);

                    temp[0] = borderFill.right;
                    printLine(Position{ maxCol - i, currPos.row }, temp, drawMode);
                }
            }
            ++currPos.row;
        }

        return;
    }

    // Otherwise print opaque Box base to console
    for (int i = 0; i < actualHeight; ++i) {
        if (i + 1 <= horizBorderSize) {
            printLine(currPos, topBorderRow.c_str(), drawMode);
        }
        else if (actualHeight - i <= horizBorderSize) {
            printLine(currPos, bottomBorderRow.c_str(), drawMode);
        }
        else {
            printLine(currPos, internalRow.c_str(), drawMode);
        }
        ++currPos.row;
    }
}

//------------------------------------------------------------------------------
//void Box::bufferBase(Position pos, Boundary container) {
//    calculateActualDimAndPos(pos, container);
//    std::string topBorderRow(actualWidth, borderFill.top);
//    std::string bottomBorderRow(actualWidth, borderFill.bottom);
//    std::string internalRow(actualWidth, ' ');
//
//    // Add borders to internalRows
//    for (int i = 0; i < vertBorderSize && i < actualWidth; ++i) {
//        topBorderRow[i] = borderFill.left;
//        bottomBorderRow[i] = borderFill.left;
//        internalRow[i] = borderFill.left;
//
//        topBorderRow[actualWidth - i - 1] = borderFill.right;
//        bottomBorderRow[actualWidth - i - 1] = borderFill.right;
//        internalRow[actualWidth - i - 1] = borderFill.right;
//    }
//
//    // Draw Box base to console
//    for (int i = 0; i < actualHeight; ++i) {
//        if (i + 1 <= horizBorderSize) {
//            console.writeToBuffer(Position{ absolutePos.col,
//                    absolutePos.row + i }, topBorderRow.c_str());
//        }
//        else if (actualHeight - i <= horizBorderSize) {
//            console.writeToBuffer(Position{ absolutePos.col,
//                    absolutePos.row + i }, bottomBorderRow.c_str());
//        }
//        else {
//            console.writeToBuffer(Position{ absolutePos.col,
//                    absolutePos.row + i }, internalRow.c_str());
//        }
//    }
//}

}