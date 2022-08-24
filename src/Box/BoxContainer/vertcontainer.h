//------------------------------------------------------------------------------
// vertcontainer.h
// Interface for the VertContainer class
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

#pragma once

#include "Box/BoxContainer/boxcontainer.h"

namespace conu {

class VertContainer : public BoxContainer {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    VertContainer();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    VertContainer(int width, int height);

    //--------------------------------------------------------------------------
    // Copy constructor
    VertContainer(const VertContainer& cpy);

    //--------------------------------------------------------------------------
    // Recieve a MouseEvent and send it to the corresponding contained Box.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this BoxContainer object and return a pointer to
    // that copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new BoxContainer object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific BoxContainer class.
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Get the target height of the BoxContainer. If the BoxContainer is 
    // dynamically sized, it will get the maximum height the BoxContainer may
    // become.
    virtual int getHeight() const override;

    //--------------------------------------------------------------------------
    // Get the target width of the BoxContainer. If the BoxContainer is 
    // dynamically sized, it will get the maximum width the BoxContainer may
    // become.
    virtual int getWidth() const override;

private:
    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) override;

    //--------------------------------------------------------------------------
    // Get the spacing vector for content printed vertically given the
    // content boundary, total height of the contents, and dynamic box count.
    std::vector<int> getSpacingHeight(const Boundary& container,
        int totalHeight, int dynamCount) const;

    //--------------------------------------------------------------------------
    // Return the calculated column offset of a Box given the Box's width. The
    // col offset is the the horizontal offset of the Box position relative to 
    // the origin of the BoxContainer.
    // Assumes that the actual dimensions of the BoxContainer have been
    // calculated.
    int getColOffset(int boxWidth) const;
};

}
