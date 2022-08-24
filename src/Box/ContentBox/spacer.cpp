//------------------------------------------------------------------------------
// spacer.cpp
// Implementation for the Spacer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Spacer is a type of ContentBox that does not hold or print
//     any internal content. Spacer objects act as padding on the console
//     screen for other Box objects. Does not produce any output or action
//     upon interaction.
//
// Class Functionality:
//     - Inherits all methods and functionality of the ContentBox class.
//     - Implements the draw() and interact() ContentBox methods.
//     - Draw the Spacer to the console window.
// 
// Dependencies: ContentBox class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/spacer.h"

namespace conu {

//------------------------------------------------------------------------------
Spacer::Spacer() :
    ContentBox() {

}

//------------------------------------------------------------------------------
Spacer::Spacer(int width, int height) :
    ContentBox(width, height) {

}

//------------------------------------------------------------------------------
Reply Spacer::interact(inputEvent::MouseEvent action) {
    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* Spacer::copyBox() const {
    return new Spacer(*this);
}

//------------------------------------------------------------------------------
Box* Spacer::createBox() const {
    return new Spacer();
}

//------------------------------------------------------------------------------
std::string Spacer::getClassName() const {
    return std::string("Spacer");
}

//------------------------------------------------------------------------------
Reply Spacer::printProtocol(Position pos, Boundary container, bool drawMode) {
    printBase(pos, container, drawMode);

    drawn = true;
    return Reply::CONTINUE;
}

}