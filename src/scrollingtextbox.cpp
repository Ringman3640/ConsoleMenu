//------------------------------------------------------------------------------
// scrollingtextbox.cpp
// Implementation for the ScrollingTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A ScrollingTextBox is a type of TextBox that allows the user-
//     defined text to be scrolled within the Box if there is not enought space
//     to display the entire text message at once. 
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/scrollingtextbox.h"

namespace conu {

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

    return Reply::REFRESH;
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
Reply ScrollingTextBox::printProtocol(Position pos, Boundary container,
        bool drawMode) {
    printBase(pos, container, drawMode);
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int temp = actualHeight - (horizBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableLines = static_cast<unsigned>(temp);

    temp = actualWidth - (vertBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableWidth = static_cast<unsigned>(temp);

    int vertOffset = 0;
    if (lines.size() < printableLines) {
        if (alignment & Align::TOP) {
            vertOffset = 0;
        }
        else if (alignment & Align::MIDDLE) {
            vertOffset = (printableLines - lines.size()) / 2;
        }
        else if (alignment & Align::BOTTOM) {
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
    Position currPos = absolutePos;
    currPos.col += vertBorderSize;
    currPos.row += vertOffset + horizBorderSize;
    for (unsigned i = 0; i < printableLines && i < lines.size(); ++i) {
        printLine(currPos, lines[i + scrollPos].c_str(), drawMode);
        ++currPos.row;
    }

    drawn = true;
    return Reply::CONTINUE;
}

}