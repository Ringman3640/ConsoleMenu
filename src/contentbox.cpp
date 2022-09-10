//------------------------------------------------------------------------------
// contentbox.cpp
// Implementation for the ContentBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A ContentBox is a type of Box object that specifically produces
//     a visual and optionally interactable element to the console screen. All
//     ContentBox objects are contained Boxes with a single executable draw()
//     and interact() method.
// 
// Dependencies: Box class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/contentbox.h"

namespace conu {

//------------------------------------------------------------------------------
ContentBox::ContentBox() :
    Box() {

}

//------------------------------------------------------------------------------
ContentBox::ContentBox(int width, int height) :
    Box(width, height) {

}

//------------------------------------------------------------------------------
int ContentBox::getHeight() const {
    return targetHeight;
}

//------------------------------------------------------------------------------
int ContentBox::getWidth() const {
    return targetWidth;
}

}