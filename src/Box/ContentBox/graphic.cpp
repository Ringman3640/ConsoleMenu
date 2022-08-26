//------------------------------------------------------------------------------
// graphic.cpp
// Implementation for the Graphic class
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
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/graphic.h"

namespace conu {

const char Graphic::DEFAULT_CANVAS_FILL = ' ';

//------------------------------------------------------------------------------
Graphic::Graphic() :
    canvas{ std::vector<char>() } {
    
    horizBorderSize = 0;
    vertBorderSize = 0;
    updateCanvasSize();
}

//------------------------------------------------------------------------------
Graphic::Graphic(int width, int height) :
    ContentBox(width, height) {
    
    horizBorderSize = 0;
    vertBorderSize = 0;
    updateCanvasSize();
}

//------------------------------------------------------------------------------
Reply Graphic::interact(inputEvent::MouseEvent action) {
    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* Graphic::copyBox() const {
    return new Graphic(*this);
}

//------------------------------------------------------------------------------
Box* Graphic::createBox() const {
    return new Graphic();
}

//------------------------------------------------------------------------------
std::string Graphic::getClassName() const {
    return std::string("Graphic");
}

//------------------------------------------------------------------------------
GraphicLine Graphic::operator [] (int idx) {
    return GraphicLine(canvas[idx]);
}

//------------------------------------------------------------------------------
GraphicLine Graphic::at(int idx) {
    if (idx < 0 || idx > canvas.size() - 1) {
        throw std::out_of_range("Index out of range in Graphic::at()");
    }

    return GraphicLine(canvas[idx]);
}

//------------------------------------------------------------------------------
void Graphic::clear() {
    for (int row = 0; row < canvas.size(); ++row) {
        std::fill(canvas[row].begin(), canvas[row].end(), ' ');
    }
}

//------------------------------------------------------------------------------
Reply Graphic::printProtocol(Position pos, Boundary container, bool drawMode) {
    printBase(pos, container, drawMode);
    if (actualWidth == 0 || actualHeight == 0) {
        return Reply::CONTINUE;
    }

    // Check for ideal drawing conditions for efficiency (that the canvas is
    //     fully visible)
    if (targetWidth == actualWidth - (vertBorderSize * 2)
        && targetHeight == actualHeight - (horizBorderSize * 2)) {
        Position currPos = absolutePos;

        for (int i = 0; i < canvas.size(); ++i) {
            canvas[i].push_back('\0');
            printLine(currPos, &canvas[i][0], drawMode);
            canvas[i].pop_back();
            ++currPos.row;
        }

        return Reply::CONTINUE;
    }

    // Canvas obscured due to resizing or borders condition
    Boundary visibleArea{
        absolutePos.col + vertBorderSize,   // Left
        absolutePos.row + horizBorderSize,  // Top
        absolutePos.col + actualWidth - 1 - vertBorderSize,     // Right
        absolutePos.row + actualHeight - 1 - horizBorderSize    // Bottom
    };
    int horizOffset = getHorizontalOffset();
    int vertOffset = getVerticalOffset();

    Position currPos{
        absolutePos.col + horizOffset,  // Col
        absolutePos.row + vertOffset    // Row
    };
    int maxRow = absolutePos.row + canvas.size();
    for (int row = 0; row < canvas.size(); ++row) {
        currPos.col = absolutePos.col + horizOffset;

        int rowSize = canvas[row].size();
        for (int col = 0; col < rowSize; ++col) {
            // Print cell if it is in the visible canvas area
            if (currPos.row >= visibleArea.top
                && currPos.row <= visibleArea.bottom
                && currPos.col >= visibleArea.left
                && currPos.col <= visibleArea.right) {
                char cStr[2] = { canvas[row][col], '\0' };
                printLine(currPos, cStr, drawMode);
            }

            ++currPos.col;
        }

        ++currPos.row;
    }

    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
void Graphic::updateCanvasSize() {
    canvas.resize(targetHeight, 
            std::vector<char>(targetWidth, DEFAULT_CANVAS_FILL));

    for (int i = 0; i < targetHeight; ++i) {
        canvas[i].resize(targetWidth, DEFAULT_CANVAS_FILL);
    }
}

//------------------------------------------------------------------------------
int Graphic::getHorizontalOffset() {
    if (alignment & Align::LEFT) {
        return vertBorderSize;
    }
    if (alignment & Align::CENTER) {
        return -(targetWidth - actualWidth) / 2;
    }
    if (alignment & Align::RIGHT) {
        return -(targetWidth - actualWidth + vertBorderSize);
    }

    // Else use LEFT alignment by default
    return vertBorderSize;
}

//------------------------------------------------------------------------------
int Graphic::getVerticalOffset() {
    if (alignment & Align::TOP) {
        return horizBorderSize;
    }
    if (alignment & Align::MIDDLE) {
        return (targetHeight - actualHeight) / 2;
    }
    if (alignment & Align::BOTTOM) {
        return -(targetHeight - actualHeight + horizBorderSize);
    }

    // Else use BOTTOM alignment by default
    return horizBorderSize;
}

//------------------------------------------------------------------------------
// GraphicLine class methods

//------------------------------------------------------------------------------
GraphicLine::GraphicLine(std::vector<char>& line) :
    canvasLine{ &line } {

}

//------------------------------------------------------------------------------
void GraphicLine::operator = (std::string lineText) {
    int idx = 0;

    int stringSize = lineText.size();
    int lineSize = canvasLine->size();
    while (idx < stringSize && idx < lineSize) {
        (*canvasLine)[idx] = lineText[idx];
        ++idx;
    }
}

//------------------------------------------------------------------------------
char& GraphicLine::operator [] (int idx) {
    return (*canvasLine)[idx];
}

//------------------------------------------------------------------------------
char& GraphicLine::at(int idx) {
    if (idx < 0 || idx > canvasLine->size() - 1) {
        throw std::out_of_range("Index out of range in GraphicsLine::at()");
    }

    return (*canvasLine)[idx];
}

}