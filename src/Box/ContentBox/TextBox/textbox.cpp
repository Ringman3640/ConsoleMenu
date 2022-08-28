//------------------------------------------------------------------------------
// textbox.cpp
// Implementation for the TextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A TextBox is a type of ContentBox that contains user-defined
//     text that is printed within the dimensions of the TextBox to the console
//     window. The text content is aligned within the TextBox depending on the
//     alignment flags specified. Does not produce any output or action upon
//     interaction.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/textbox.h"

namespace conu {

//------------------------------------------------------------------------------
TextBox::TextBox(std::string text) :
    ContentBox(),
    text{ text } {

}

//------------------------------------------------------------------------------
TextBox::TextBox(int width, int height, std::string text) :
    ContentBox(width, height),
    text{ text } {

}

//------------------------------------------------------------------------------
Reply TextBox::interact(inputEvent::MouseEvent action) {
    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* TextBox::copyBox() const {
    return new TextBox(*this);
}

//------------------------------------------------------------------------------
Box* TextBox::createBox() const {
    return new TextBox();
}

//------------------------------------------------------------------------------
std::string TextBox::getClassName() const {
    return std::string("TextBox");
}

//------------------------------------------------------------------------------
void TextBox::setText(std::string text) {
    this->text = text;
}

//------------------------------------------------------------------------------
Reply TextBox::printProtocol(Position pos, Boundary container, bool drawMode) {
    printBase(pos, container, drawMode);
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int temp = actualHeight - (horizBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableLines = static_cast<unsigned>(temp);
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

    // Print text
    Position currPos = absolutePos;
    currPos.col += vertBorderSize;
    currPos.row += vertOffset + horizBorderSize;
    for (unsigned i = 0; i < printableLines && i < lines.size(); ++i) {
        printLine(currPos, lines[i].c_str(), drawMode);
        ++currPos.row;
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
void TextBox::splitText() {
    int startIdx = 0;
    int endIdx = 0;
    lines.clear();

    // Check if there is no printable width for text
    if (actualWidth - (vertBorderSize * 2) <= 0) {
        return;
    }

    for (unsigned i = 0; i < text.size(); ++i) {

        // Check if current range is larger than the allottable content width
        int temp = actualWidth - (vertBorderSize * 2);
        if (temp < 0) {
            temp = 0;
        }
        unsigned contentWidth = static_cast<unsigned>(temp);
        if (i - startIdx >= contentWidth) {

            // Check if current range is one large word
            if (startIdx == endIdx) {
                // Set endIdx to max range index
                endIdx = i - 1;
            }

            lines.push_back(text.substr(startIdx, endIdx - startIdx));
            startIdx = endIdx + 1;
            endIdx = startIdx;
        }

        // Check for word split
        else if (text[i] == ' ') {
            endIdx = i;
        }
    }

    // Append remaining words if necessary
    if (startIdx != text.size() - 1) {
        lines.push_back(text.substr(startIdx));
    }
}

//------------------------------------------------------------------------------
void TextBox::applyHorizontalAlignment() {
    int contentWidth = actualWidth - (vertBorderSize * 2);

    if (alignment & Align::LEFT) {
        return;
    }
    if (alignment & Align::CENTER) {
        for (unsigned i = 0; i < lines.size(); ++i) {
            int padding = (contentWidth - lines[i].size()) / 2;
            lines[i] = std::string(padding, ' ') + lines[i];
        }
        return;
    }
    if (alignment & Align::RIGHT) {
        for (unsigned i = 0; i < lines.size(); ++i) {
            int padding = (contentWidth - lines[i].size());
            lines[i] = std::string(padding, ' ') + lines[i];
        }
        return;
    }
}

}