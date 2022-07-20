//------------------------------------------------------------------------------
// boxcontainer.cpp
// Implementation for the BoxContainer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A BoxContainer is a type of Box that contains and manages other
//     Box objects. BoxContainers are responsible for printing all contained
//     Boxes within its boundary according to the specified Alignment and
//     BoxDistrib flags applied. Any input MouseEvent is passed into the 
//     corresponding contained Box and returns its Reply.
//
// Class Functionality:
//     - Inherits all methods and functionality of the Box class.
//     - Insert Boxes into the BoxContainer.
//     - Remove Boxes from the BoxContainer.
//     - Get references to Boxes within the BoxContainer.
//     - Set if the Box is dynamically sized (if its dimensions can grow based
//     - on the size of its contents).
//     - Set the distribution of Boxes contained within the BoxContainer.
// 
// Dependencies: Box class.
//------------------------------------------------------------------------------

#include "Box/BoxContainer/boxcontainer.h"

namespace conu {

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
BoxContainer::~BoxContainer() {
    clearContents();
}

//------------------------------------------------------------------------------
ItemAccessor BoxContainer::operator[] (int layer) {
    return ItemAccessor(*this, layer);
}

//------------------------------------------------------------------------------
void BoxContainer::insert(int layer, const Box& inBox) {
    if (contents.find(layer) != contents.end()) {
        delete contents[layer].item;
        contents.erase(layer);
    }

    contents[layer] = BoxItem{ inBox.copyBox(), false, Position{-1, -1} };
    updateHeightWidth = true;
}

//------------------------------------------------------------------------------
void BoxContainer::insert(int layer, const Box& inBox, const Position& pos) {
    if (contents.find(layer) != contents.end()) {
        delete contents[layer].item;
        contents.erase(layer);
    }

    contents[layer] = BoxItem{ inBox.copyBox(), true, pos };
    updateHeightWidth = true;
}

//------------------------------------------------------------------------------
void BoxContainer::remove(int layer) {
    auto it = contents.find(layer);
    if (it == contents.end()) {
        return;
    }

    delete it->second.item;
    contents.erase(layer);
}

//------------------------------------------------------------------------------
Box* BoxContainer::get(int layer) const {
    auto target = contents.find(layer);
    if (target == contents.end()) {
        return nullptr;
    }

    return target->second.item;
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

//------------------------------------------------------------------------------
void BoxContainer::clearContents() {
    while (!contents.empty()) {
        delete contents.begin()->second.item;
        contents.erase(contents.begin()->first);
    }
}

//------------------------------------------------------------------------------
// ItemAccessor Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ItemAccessor::ItemAccessor(BoxContainer& container, int layer) :
    container{ &container },
    layer{ layer } {

}

//------------------------------------------------------------------------------
void ItemAccessor::operator = (const Box& inBox) {
    container->insert(layer, inBox);
}

//------------------------------------------------------------------------------
void ItemAccessor::insert(const Box& inBox) {
    container->insert(layer, inBox);
}

//------------------------------------------------------------------------------
void ItemAccessor::insert(const Box& inBox, const Position& pos) {
    container->insert(layer, inBox, pos);
}

//------------------------------------------------------------------------------
void ItemAccessor::remove() {
    container->remove(layer);
}

//------------------------------------------------------------------------------
Box* ItemAccessor::get() {
    return container->get(layer);
}

}