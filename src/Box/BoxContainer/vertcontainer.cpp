//------------------------------------------------------------------------------
// vertcontainer.cpp
// Implementation for the VertContainer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A VertContainer is a type of Box that contains and manages
//     other Box objects. VertContainer are responsible for printing all
//     contained Boxes vertically within its boundary according to the specified
//     Alignment and BoxDistrib flags applied. Any input MouseEvent is passed
//     into the corresponding contained Box and returns its Reply
//
// Class Functionality:
//     - Inherits all methods and functionality of the BoxContainer class.
//     - Implements the draw() and interact() BoxContainer methods.
//     - Draw the VertContainer to the console window.
// 
// Dependencies: BoxContainer class.
//------------------------------------------------------------------------------

#include "Box/BoxContainer/vertcontainer.h"

namespace conu {

//------------------------------------------------------------------------------
VertContainer::VertContainer() :
    BoxContainer() {

}

//------------------------------------------------------------------------------
VertContainer::VertContainer(int width, int height) :
    BoxContainer(width, height) {

}

//------------------------------------------------------------------------------
VertContainer::VertContainer(
    const VertContainer& cpy) :
    BoxContainer(cpy) {

    for (auto it = cpy.contents.begin(); it != cpy.contents.end(); ++it) {
        this->contents[it->first].item = it->second.item->copyBox();
    }
}

//------------------------------------------------------------------------------
Reply VertContainer::draw(Position pos, Boundary container) {
    // Get acutal dimensions and print base
    int prevTargWidth = targetWidth;
    int prevTargHeight = targetHeight;
    targetWidth = getWidth();
    targetHeight = getHeight();
    printBase(pos, container);
    targetWidth = prevTargWidth;
    targetHeight = prevTargHeight;

    // Check if there is anything to print
    if (contents.empty()) {
        return Reply::CONTINUE;
    }

    // Get total content height
    int totalHeight = 0;
    int dynamCount = 0;
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        if (!it->second.fixed) {
            totalHeight += it->second.item->getHeight();
            ++dynamCount;
        }
    }

    // Get content space boundary and width spacing
    Boundary contentBound = getContentBound(absolutePos);
    std::vector<int> spacing = getSpacingHeight(contentBound, totalHeight,
        dynamCount);

    // Print contents
    int spacingIdx = spacing.size() - 1;
    Position offset{ 0, actualHeight - horizBorderSize - 0 }; // TODO: test the - 1
    for (auto it = contents.rbegin(); it != contents.rend(); ++it) {
        if (it->second.fixed) {
            Position drawPos{ it->second.pos.col + pos.col, it->second.pos.row
                    + pos.row };
            it->second.item->draw(drawPos, contentBound);
        }
        else {
            int itemHeight = it->second.item->getHeight();
            offset.row -= spacing[spacingIdx--] + itemHeight + 0; // TODO: test the + 1
            offset.col = getColOffset(it->second.item->getWidth());
            it->second.item->draw(Position{ absolutePos.col + offset.col,
                    absolutePos.row + offset.row }, contentBound);
        }
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply VertContainer::buffer(Position pos, Boundary container) {
    // Get acutal dimensions and print base
    int prevTargWidth = targetWidth;
    int prevTargHeight = targetHeight;
    targetWidth = getWidth();
    targetHeight = getHeight();
    bufferBase(pos, container);
    targetWidth = prevTargWidth;
    targetHeight = prevTargHeight;

    // Check if there is anything to print
    if (contents.empty()) {
        return Reply::CONTINUE;
    }

    // Get total content height
    int totalHeight = 0;
    int dynamCount = 0;
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        if (!it->second.fixed) {
            totalHeight += it->second.item->getHeight();
            ++dynamCount;
        }
    }

    // Get content space boundary and width spacing
    Boundary contentBound = getContentBound(absolutePos);
    std::vector<int> spacing = getSpacingHeight(contentBound, totalHeight,
        dynamCount);

    // Print contents
    int spacingIdx = spacing.size() - 1;
    Position offset{ 0, actualHeight - horizBorderSize - 0 }; // TODO: test the - 1
    for (auto it = contents.rbegin(); it != contents.rend(); ++it) {
        if (it->second.fixed) {
            Position drawPos{ it->second.pos.col + pos.col, it->second.pos.row
                    + pos.row };
            it->second.item->buffer(drawPos, contentBound);
        }
        else {
            int itemHeight = it->second.item->getHeight();
            offset.row -= spacing[spacingIdx--] + itemHeight + 0; // TODO: test the + 1
            offset.col = getColOffset(it->second.item->getWidth());
            it->second.item->buffer(Position{ absolutePos.col + offset.col,
                    absolutePos.row + offset.row }, contentBound);
        }
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply VertContainer::interact(inputEvent::MouseEvent action) {
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        if (it->second.item->posInBounds(action.mousePosition)) {
            return it->second.item->interact(action);
        }
    }

    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* VertContainer::copyBox() const {
    return new VertContainer(*this);
}

//------------------------------------------------------------------------------
Box* VertContainer::createBox() const {
    return new VertContainer();
}

//------------------------------------------------------------------------------
std::string VertContainer::getClassName() const {
    return std::string("VertContainer");
}

//------------------------------------------------------------------------------
int VertContainer::getHeight() const {
    if (!updateHeightWidth) {
        return returnHeight;
    }
    if (!dynamicSized) {
        //updateHeightWidth = false;
        return targetHeight;
    }

    int maxHeight = 0;
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        //maxHeight = std::max<int>(maxHeight, it->second.item->getHeight());
        maxHeight += it->second.item->getHeight();
    }
    maxHeight += horizBorderSize * 2;

    //updateHeightWidth = false;
    return std::max<int>(targetHeight, maxHeight);
}

//------------------------------------------------------------------------------
int VertContainer::getWidth() const {
    if (!updateHeightWidth) {
        return returnWidth;
    }
    if (!dynamicSized) {
        //updateHeightWidth = false;
        return targetWidth;
    }

    int maxWidth = 0;
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        //maxWidth += it->second.item->getWidth();
        maxWidth = std::max<int>(maxWidth, it->second.item->getWidth());
    }
    maxWidth += vertBorderSize * 2;

    //updateHeightWidth = false;
    return std::max<int>(targetWidth, maxWidth);
}

//------------------------------------------------------------------------------
std::vector<int> VertContainer::getSpacingHeight(
    const Boundary& container, int totalHeight, int dynamCount) const {
    int contentHeight = container.bottom - container.top + 1;
    int totalSpace = contentHeight - totalHeight;
    std::vector<int> spacing;

    int estimate, remainder;
    switch (distribution) {
    case BoxDistrib::EVEN_SPACED:
        // Get spacing estimate
        estimate = totalSpace / (dynamCount + 1);
        spacing.resize(dynamCount + 1, estimate);

        // Distribute remainder
        remainder = totalSpace % (dynamCount + 1);
        for (int i = 0; i < remainder; ++i) {
            ++spacing[i];
        }
        break;

    case BoxDistrib::SPACED_TO_EDGE:
        // Check edge case
        if (dynamCount <= 1) {
            int estimate = totalSpace / 2;
            return std::move(std::vector<int>(2, estimate));
        }

        // Get spacing estimate
        estimate = totalSpace / (dynamCount - 1);
        spacing.resize(dynamCount, estimate);
        spacing[0] = 0;

        // Distribute remainder
        remainder = (dynamCount == 1) ? totalSpace :
            totalSpace % (dynamCount - 1);
        for (int i = 0; i < remainder; ++i) {
            ++spacing[i + 1]; // + 1 to skip first index
        }
        spacing.push_back(0);
        break;

    case BoxDistrib::NONE:
    default:
        spacing.resize(dynamCount, 0);

        // Check edge case
        if (dynamCount == 0) {
            return std::move(spacing);
        }

        if (alignment & Align::LEFT) {
            spacing[spacing.size() - 1] = totalSpace;
        }
        else if (alignment & Align::RIGHT) {
            // do nothing
        }
        else {
            // Default alignment is CENTER
            spacing[spacing.size() - 1] = totalSpace / 2;
        }
        break;
    }

    return std::move(spacing);
}

//------------------------------------------------------------------------------
int VertContainer::getColOffset(int boxWidth) const {
    if (alignment & Align::LEFT) {
        return 0;
    }
    if (alignment & Align::RIGHT) {
        return actualWidth - boxWidth;
    }

    // Default alignment is MIDDLE
    return (actualWidth - boxWidth) / 2;
}

}