//------------------------------------------------------------------------------
// horizontalboxcontainer.cpp
// Implementation for the HorizontalBoxContainer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A HorizontalBoxContainer is a type of Box that contains and
//     manages other Box objects. HorizontalBoxContainers are responsible for
//     printing all contained Boxes horizontally within its boundary according
//     to the specified Alignment and BoxDistrib flags applied. Any input 
//     MouseEvent is passed into the corresponding contained Box and returns its
//     Reply.
//
// Class Functionality:
//     - 
//     - 
//     - 
// 
// Dependencies: BoxContainer class.
//------------------------------------------------------------------------------

#include "Box/BoxContainer/horizontalboxcontainer.h"

//------------------------------------------------------------------------------
    // Default constructor
HorizontalBoxContainer::HorizontalBoxContainer() :
    BoxContainer() {

}

//------------------------------------------------------------------------------
// Parameterized constructor
HorizontalBoxContainer::HorizontalBoxContainer(int width, int height) :
    BoxContainer(width, height) {

}

//------------------------------------------------------------------------------
HorizontalBoxContainer::HorizontalBoxContainer(
        const HorizontalBoxContainer& cpy) :
    BoxContainer(cpy) {

    for (int i = 0; i < cpy.contents.size(); ++i) {
        this->contents[i].item = cpy.contents[i].item->copyBox();
    }
}

//------------------------------------------------------------------------------
Reply HorizontalBoxContainer::draw(Position pos, Boundary container) {
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

    // Get total content width
    int totalWidth = 0;
    int dynamCount = 0;
    for (int i = 0; i < contents.size(); ++i) {
        if (!contents[i].fixed) {
            totalWidth += contents[i].item->getWidth();
            ++dynamCount;
        }
    }

    // Get content space boundary and width spacing
    Boundary contentBound = getContentBound(pos);
    std::vector<int> spacing = getSpacingWidth(contentBound, totalWidth, 
            dynamCount);

    // TODO: printing inconsistenty
    // Problem: no need to subtract verBorderSize from starting offset position value (messes up contents printing since 
    // many need to reconsider how Boxes are printed and if the given is position is relative to the given boundary

    // Print contents
    int spacingIdx = spacing.size() - 1;
    Position offset{ actualWidth - vertBorderSize - 0, 0 }; // TODO: test the - 1
    for (int i = contents.size() - 1; i >= 0; --i) {
        if (contents[i].fixed) {
            Position drawPos{ contents[i].pos.col + pos.col, 
                    contents[i].pos.row + pos.row };
            contents[i].item->draw(drawPos, contentBound);
        }
        else {
            int itemWidth = contents[i].item->getWidth();
            offset.col -= spacing[spacingIdx--] + itemWidth + 0; // TODO: test the + 1
            offset.row = getRowOffset(contents[i].item->getHeight());
            contents[i].item->draw(Position{ offset.col, offset.row }, 
                    contentBound);
        }
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
Reply HorizontalBoxContainer::buffer(Position pos, Boundary container) {
    // stub
    return Reply::FAILED;
}

//------------------------------------------------------------------------------
Reply HorizontalBoxContainer::interact(inputEvent::MouseEvent action) {
    for (int i = 0; i < contents.size(); ++i) {
        if (contents[i].item->posInBounds(action.mousePosition)) {
            return contents[i].item->interact(action);
        }
    }

    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* HorizontalBoxContainer::copyBox() const {
    return new HorizontalBoxContainer(*this);
}

//------------------------------------------------------------------------------
Box* HorizontalBoxContainer::createBox() const {
    return new HorizontalBoxContainer();
}

//------------------------------------------------------------------------------
std::string HorizontalBoxContainer::getClassName() const {
    return std::string("HorizontalBoxContainer");
}

//------------------------------------------------------------------------------
int HorizontalBoxContainer::getHeight() const {
    if (!updateHeightWidth) {
        return returnHeight;
    }
    if (!dynamicSized) {
        //updateHeightWidth = false;
        return targetHeight;
    }

    int maxHeight = 0;
    for (int i = 0; i < contents.size(); ++i) {
        maxHeight = std::max<int>(maxHeight, contents[i].item->getHeight());
    }
    maxHeight += horizBorderSize;

    //updateHeightWidth = false;
    return std::max<int>(targetHeight, maxHeight);
}

//------------------------------------------------------------------------------
int HorizontalBoxContainer::getWidth() const {
    if (!updateHeightWidth) {
        return returnWidth;
    }
    if (!dynamicSized) {
        //updateHeightWidth = false;
        return targetWidth;
    }

    int maxWidth = 0;
    for (int i = 0; i < contents.size(); ++i) {
        maxWidth = std::max<int>(maxWidth, contents[i].item->getHeight());
    }
    maxWidth += vertBorderSize;

    //updateHeightWidth = false;
    return std::max<int>(targetWidth, maxWidth);
}

//------------------------------------------------------------------------------
std::vector<int> HorizontalBoxContainer::getSpacingWidth(
        const Boundary& container, int totalWidth, int dynamCount) const {
    int contentWidth = container.right - container.left + 1;
    int totalSpace = contentWidth - totalWidth;
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

        if (align & Alignment::LEFT) {
            spacing[spacing.size() - 1] = totalSpace;
        }
        else if (align & Alignment::RIGHT) {
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
int HorizontalBoxContainer::getRowOffset(int boxHeight) const {
    if (align & Alignment::TOP) {
        return 0;
    }
    if (align & Alignment::BOTTOM) {
        return actualHeight - boxHeight;
    }

    // Default alignment is MIDDLE
    return (actualHeight - boxHeight) / 2;
}