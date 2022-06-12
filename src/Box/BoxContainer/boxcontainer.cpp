//------------------------------------------------------------------------------
// boxcontainer.cpp
// Implementation for the BoxContainer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: 
//
// Class Functionality:
//     - 
//     - 
//     - 
// 
// Dependencies: 
//------------------------------------------------------------------------------

#include "Box/BoxContainer/boxcontainer.h"

//------------------------------------------------------------------------------
BoxContainer::BoxContainer() :
    Box(),
    distribution{ BoxDistrib::NONE },
    returnHeight{ 0 },
    returnWidth{ 0 },
    updateHeightWidth{ true },
    dynamicSized{ false } {

}

//------------------------------------------------------------------------------
BoxContainer::BoxContainer(int width, int height) :
    Box(width, height),
    distribution{ BoxDistrib::NONE },
    returnHeight{ 0 },
    returnWidth{ 0 },
    updateHeightWidth{ true },
    dynamicSized{ false } {

}

//------------------------------------------------------------------------------
void BoxContainer::insert(Box& inBox) {
    contents.push_back(BoxItem{ &inBox, false, Position{-1, -1} });
    updateHeightWidth = true;
}

//------------------------------------------------------------------------------
void BoxContainer::insert(Box& inBox, Position pos) {
    contents.push_back(BoxItem{ &inBox, true, pos });
    updateHeightWidth = true;
}

//------------------------------------------------------------------------------
void BoxContainer::dynamicallySized(bool set) {
    updateHeightWidth = true;
    dynamicSized = set;
}

//------------------------------------------------------------------------------
void BoxContainer::setDistribution(BoxDistrib distribution) {
    this->distribution = distribution;
}

//------------------------------------------------------------------------------
Boundary BoxContainer::getContentBound(const Position& pos) const {
    return Boundary{ pos.col + vertBorderSize,
            pos.row + horizBorderSize,
            pos.col + actualWidth - vertBorderSize - 1,
            pos.row + actualHeight - horizBorderSize - 1 };
}

//------------------------------------------------------------------------------
std::vector<int> BoxContainer::getSpacingHeight(const Boundary& container,
        int totalHeight, int dynamCount) const {
    int contentHeight = container.bottom - container.top;
    int remainingSpace = contentHeight - totalHeight;
    std::vector<int> spacing;

    int estimate, remainder;
    switch (distribution) {
    case BoxDistrib::EVEN_SPACED:
        // Get spacing estimate
        estimate = remainingSpace / (dynamCount + 1);
        spacing.resize(dynamCount + 1, estimate);

        // Distribute remainder
        remainder = remainingSpace % estimate;
        for (int i = 0; i < remainder; ++i) {
            ++spacing[i];
        }
        break;

    case BoxDistrib::SPACED_TO_EDGE:
        // Get spacing estimate
        estimate = remainingSpace / (dynamCount - 1);
        spacing.resize(dynamCount - 1, estimate);

        // Distribute remainder
        remainder = remainingSpace % estimate;
        for (int i = 0; i < remainder; ++i) {
            ++spacing[i];
        }
        break;

    case BoxDistrib::NONE:
        // No spacing
        break;

    default:
        break;
    }

    return std::move(spacing);
}