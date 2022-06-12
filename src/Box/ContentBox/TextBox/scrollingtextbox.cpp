//------------------------------------------------------------------------------
// scrollingtextbox.cpp
// Implementation for the ScrollingTextBox class
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

#include "Box/ContentBox/TextBox/scrollingtextbox.h"

//------------------------------------------------------------------------------
ScrollingTextBox::ScrollingTextBox() :
    TextBox(),
    scrollPos{ 0 } {

}

//------------------------------------------------------------------------------
ScrollingTextBox::ScrollingTextBox(int width, int height, 
        std::string text) :
    TextBox(width, height, text),
    scrollPos{ 0 } {

}

//------------------------------------------------------------------------------
Reply ScrollingTextBox::draw(Position pos, Boundary container) {
    printBase(pos, container);

    drawScrollTBox();

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply ScrollingTextBox::buffer(Position pos, Boundary container) {
    bufferBase(pos, container);

    bufferScrollTBox();

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply ScrollingTextBox::interact(inputEvent::MouseEvent action) {
    // Make sure TextBox has already been drawn
    if (!drawn) {
        return Reply::FAILED;
    }

    // Make sure input is within the ContentBox boundary
    if (!posInBounds(action.mousePosition)) {
        return Reply::IGNORED;
    }

    if (action.eventFlag == inputEvent::Mouse::WHEELED_BACKWARD) {
        ++scrollPos;
    }
    else if (action.eventFlag == inputEvent::Mouse::WHEELED_FORWARD) {
        --scrollPos;
    }
    else {
        return Reply::IGNORED;
    }

    drawScrollTBox();
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Box* ScrollingTextBox::copyBox() const {
    return new ScrollingTextBox(*this);
}

//------------------------------------------------------------------------------
Box* ScrollingTextBox::createBox() const {
    return new ScrollingTextBox();
}

//------------------------------------------------------------------------------
std::string ScrollingTextBox::getClassName() const {
    return std::string("ScrollingTextBox");
}

//------------------------------------------------------------------------------
void ScrollingTextBox::drawScrollTBox() {
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int printableLines = actualHeight - (horizBorderSize * 2);
    int printableWidth = actualWidth - (vertBorderSize * 2);

    int vertOffset = 0;
    if (lines.size() < printableLines) {
        if (align & Alignment::TOP) {
            vertOffset = 0;
        }
        else if (align & Alignment::MIDDLE) {
            vertOffset = (printableLines - lines.size()) / 2;
        }
        else if (align & Alignment::BOTTOM) {
            vertOffset = printableLines - lines.size();
        }
    }

    // Check scrollPos value
    if (lines.size() <= printableLines) {
        scrollPos = 0;
    }
    else if (scrollPos < 0) {
        scrollPos = 0;
    }
    else if (lines.size() - scrollPos < printableLines) {
        scrollPos = lines.size() - printableLines;
    }

    // Print text
    std::string clearLine(printableWidth, ' ');
    int startRow = vertOffset + absolutePos.row;
    for (int i = 0; i < printableLines && i < lines.size(); ++i) {
        console.setCursorPosition(Position{ absolutePos.col + vertBorderSize, 
                startRow + horizBorderSize + i });
        std::cout << clearLine;
        console.setCursorPosition(Position{ absolutePos.col + vertBorderSize, 
                startRow + horizBorderSize + i });
        std::cout << lines[i + scrollPos];
    }
}

//------------------------------------------------------------------------------
void ScrollingTextBox::bufferScrollTBox() {
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int printableLines = actualHeight - (horizBorderSize * 2);
    int printableWidth = actualWidth - (vertBorderSize * 2);

    int vertOffset = 0;
    if (lines.size() < printableLines) {
        if (align & Alignment::TOP) {
            vertOffset = 0;
        }
        else if (align & Alignment::MIDDLE) {
            vertOffset = (printableLines - lines.size()) / 2;
        }
        else if (align & Alignment::BOTTOM) {
            vertOffset = printableLines - lines.size();
        }
    }

    // Check scrollPos value
    if (lines.size() <= printableLines) {
        scrollPos = 0;
    }
    else if (scrollPos < 0) {
        scrollPos = 0;
    }
    else if (lines.size() - scrollPos < printableLines) {
        scrollPos = lines.size() - printableLines;
    }

    // Print text
    std::string clearLine(printableWidth, ' ');
    int startRow = vertOffset + absolutePos.row;
    for (int i = 0; i < printableLines && i < lines.size(); ++i) {
        console.writeToBuffer(Position{ absolutePos.col + vertBorderSize,
                startRow + horizBorderSize + i }, clearLine.c_str());

        console.writeToBuffer(Position{ absolutePos.col + vertBorderSize,
                startRow + horizBorderSize + i }, lines[i + scrollPos].c_str());
    }
}