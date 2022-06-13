//------------------------------------------------------------------------------
// boxcontainer.h
// Interface for the BoxContainer class
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
//     - Set if the Box is dynamically sized (if its dimensions can grow based
//     - on the size of its contents).
//     - Set the distribution of Boxes contained within the BoxContainer.
// 
// Dependencies: Box class.
//------------------------------------------------------------------------------

#pragma once

#include <vector>
#include <algorithm>
#include "Box/box.h"

//------------------------------------------------------------------------------
// BoxDistrib enumerator
// Indicates how Boxes within the BoxContainer should be distributed along its
// length. 
enum class BoxDistrib {
    EVEN_SPACED,        // Spaces of equal width next to each Box.
    SPACED_TO_EDGE,     // Similar to EVEN_SPACED, but there is no space between 
                        // the edge of the BoxContainer and the internal Boxes.
    NONE                // No spacing, all internal Boxes are touching. 
};

//------------------------------------------------------------------------------
class BoxContainer : public Box {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    BoxContainer();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    BoxContainer(int width, int height);

    //--------------------------------------------------------------------------
    // Draw the contents of the BoxContainer to the output console given an
    // origin column and row, and a constraining rectangle that represents the
    // container boundaries that the BoxContainer is within.
    virtual Reply draw(Position pos, Boundary container) = 0;

    //--------------------------------------------------------------------------
    // Buffer the contents BoxContainer to EditConsole's write buffer given an
    // origin column and row, and a constraining rectangle that represents the
    // container boundaries that the BoxContainer is within.
    virtual Reply buffer(Position pos, Boundary container) = 0;

    //--------------------------------------------------------------------------
    // Recieve a MouseEvent and send it to the corresponding contained Box.
    virtual Reply interact(inputEvent::MouseEvent action) = 0;

    //--------------------------------------------------------------------------
    // Create a deep copy of this BoxContainer object and return a pointer to
    // that copy.
    virtual Box* copyBox() const = 0;

    //--------------------------------------------------------------------------
    // Create a new BoxContainer object and return a pointer to that object.
    virtual Box* createBox() const = 0;

    //--------------------------------------------------------------------------
    // Get the name of the specific BoxContainer class.
    virtual std::string getClassName() const = 0;

    //--------------------------------------------------------------------------
    // Get the target height of the BoxContainer. If the BoxContainer is 
    // dynamically sized, it will get the maximum height the BoxContainer may
    // become.
    virtual int getHeight() const = 0;

    //--------------------------------------------------------------------------
    // Get the target width of the BoxContainer. If the BoxContainer is 
    // dynamically sized, it will get the maximum width the BoxContainer may
    // become.
    virtual int getWidth() const = 0;

    //--------------------------------------------------------------------------
    // Insert a Box into the BoxContainer. The Box is marked as "dynamic",
    // meaning that the Box is positioned according to the BoxContainer's 
    // alignment and distribution flags.
    virtual void insert(Box& inBox);

    //--------------------------------------------------------------------------
    // Insert a Box into the BoxContainer at a specific position. The Box is 
    // marked as "fixed", meaning that the the presence of the Box is not 
    // affected by the alignment and distribution flags of the BoxContainer, and
    // the distribution of the dynamic Boxes are not affected by the inserted
    // Box.
    virtual void insert(Box& inBox, Position pos);

    //--------------------------------------------------------------------------
    // Set whether or not the BoxContainer is dynamically sized. A dynamically
    // sized box will change its dimensions to fit the combined size of its
    // contents, up to the size of the containing Boundary. Otherwise, the 
    // BoxContainer has a fixed size and will not increase its dimensions to fit
    // its contents.
    void dynamicallySized(bool set);

    //--------------------------------------------------------------------------
    // Set the content distribution of the BoxContainer.
    void setDistribution(BoxDistrib distribution);

protected:
    //--------------------------------------------------------------------------
    // BoxItem structure
    // Contains information about an internal Box within the BoxContainer, such
    // as if the Box is fixed at a set position, and the position of the Box
    // if it is fixed.
    struct BoxItem {
        Box* item;
        bool fixed;
        Position pos;
    };

    std::vector<BoxItem> contents;
    BoxDistrib distribution;
    int returnHeight;
    int returnWidth;
    mutable bool updateHeightWidth;
    bool dynamicSized;

    //--------------------------------------------------------------------------
    // Get the content boundary of the BoxContainer (accounting for horizontal
    // and vertical border sizes) given the position of the BoxContainer.
    Boundary getContentBound(const Position& pos) const;

    //--------------------------------------------------------------------------
    // Get the spacing vector for content printed vertically given the
    // content boundary, total height of the contents, and dynamic box count.
    std::vector<int> getSpacingHeight(const Boundary& container,
        int totalHeight, int dynamCount) const;

};