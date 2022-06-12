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
// Class Functionality:
//     - Inherits all methods and functionality of the ContentBox class.
//     - Implements the draw() and interact() ContentBox methods.
//     - Draw the TextBox object to the console window.
//     - Set the text of the TextBox that will be printed.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/textbox.h"

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
Reply TextBox::draw(Position pos, Boundary container) {
    printBase(pos, container);
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int printableLines = actualHeight - (horizBorderSize * 2);
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

    // Print text
    int startRow = vertOffset + absolutePos.row;
    for (int i = 0; i < printableLines && i < lines.size(); ++i) {
        console.setCursorPosition(Position{ absolutePos.col + vertBorderSize,
            startRow + horizBorderSize + i });
        std::cout << lines[i];
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply TextBox::buffer(Position pos, Boundary container) {
    bufferBase(pos, container);
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int printableLines = actualHeight - (horizBorderSize * 2);
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

    // Print text
    int startRow = vertOffset + absolutePos.row;
    for (int i = 0; i < printableLines && i < lines.size(); ++i) {
        console.writeToBuffer(Position{ absolutePos.col + vertBorderSize, 
                startRow + horizBorderSize + i }, lines[i].c_str());
    }

    drawn = true;
    return Reply::CONTINUE;
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
void TextBox::splitText() {
    int startIdx = 0;
    int endIdx = 0;
    lines.clear();

    // Check if there is no printable width for text
    if (actualWidth - (vertBorderSize * 2) <= 0) {
        return;
    }

    for (int i = 0; i < text.size(); ++i) {

        // Check if current range is larger than the allottable content width
        if (i - startIdx >= actualWidth - (vertBorderSize * 2)) {

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

    if (align & Alignment::LEFT) {
        return;
    }
    if (align & Alignment::CENTER) {
        for (int i = 0; i < lines.size(); ++i) {
            int padding = (contentWidth - lines[i].size()) / 2;
            lines[i] = std::string(padding, ' ') + lines[i];
        }
        return;
    }
    if (align & Alignment::RIGHT) {
        for (int i = 0; i < lines.size(); ++i) {
            int padding = (contentWidth - lines[i].size());
            lines[i] = std::string(padding, ' ') + lines[i];
        }
        return;
    }
}