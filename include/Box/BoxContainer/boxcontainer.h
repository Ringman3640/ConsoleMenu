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
// Dependencies: Box class.
//------------------------------------------------------------------------------

#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include "Box/box.h"

namespace conu {

class BoxContainer;

//------------------------------------------------------------------------------
// ItemAccessor class
// Interface for users to directly add or remove Box items to the container
//     through the bracket operator. Helper class for BoxContainer.
class ItemAccessor {
public:
    //--------------------------------------------------------------------------
    // Constructor
    ItemAccessor(BoxContainer& container, int layer);

    //--------------------------------------------------------------------------
    // Assignment operator. Insert a dynamic Box into the target BoxContainer at
    // the given layer.
    void operator = (const Box& inBox);

    //--------------------------------------------------------------------------
    // Insert a dynamic Box into the target BoxContainer at the given layer.
    void insert(const Box& inBox);

    //--------------------------------------------------------------------------
    // Insert a static Box into the target BoxContainer at the given layer.
    void insert(const Box& inBox, const Position& pos);

    //--------------------------------------------------------------------------
    // Remove the Box located at a given layer in the target BoxContainer.
    void remove();

    //--------------------------------------------------------------------------
    // Get a pointer reference to a contained Box item.
    // Returns nullptr if there was no Box.
    Box* get();

private:
    BoxContainer* container;
    int layer;

};

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
    // Virtual destructor
    virtual ~BoxContainer() override;

    //--------------------------------------------------------------------------
    // Obtain an ItemAccessor for a specific layer within the BoxContainer.
    ItemAccessor operator [] (int layer);

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
    virtual int getHeight() const;

    //--------------------------------------------------------------------------
    // Get the total maximum height of the BoxContainer's inner contents.
    virtual int getContentHeight() const = 0;

    //--------------------------------------------------------------------------
    // Get the target width of the BoxContainer. If the BoxContainer is 
    // dynamically sized, it will get the maximum width the BoxContainer may
    // become.
    virtual int getWidth() const;

    //--------------------------------------------------------------------------
    // Get the total maximum width of the BoxContainer's inner contents.
    virtual int getContentWidth() const = 0;

    //--------------------------------------------------------------------------
    // Insert a Box into the BoxContainer at the next available layer
    // incrementally starting from layer 1. The Box is marked as "dynamic".
    virtual void insert(const Box& inBox);

    //--------------------------------------------------------------------------
    // Insert a Box into the BoxContainer at a given layer. The Box is marked as
    // "dynamic", meaning that the Box is positioned according to the
    // BoxContainer's alignment and distribution flags.
    virtual void insert(int layer, const Box& inBox);

    //--------------------------------------------------------------------------
    // Insert a Box into the BoxContainer at a specific position at a given
    // layer. The Box is marked as "fixed", meaning that the the presence of the
    // Box is not affected by the alignment and distribution flags of the
    // BoxContainer, and the distribution of the dynamic Boxes are not affected
    // by the inserted Box.
    virtual void insert(int layer, const Box& inBox, const Position& pos);

    //--------------------------------------------------------------------------
    // Remove a Box from the BoxContainer at a specified layer. 
    virtual void remove(int layer);

    //--------------------------------------------------------------------------
    // Get a pointer reference to a contained Box item given its layer value. 
    // Returns nullptr if there was no Box found at the given layer.
    virtual Box* get(int layer) const;

    //--------------------------------------------------------------------------
    // Get a pointer reference to the most recently inserted Box item within the
    // BoxContainer. Returns nullptr if no Boxes have been inserted, or if the
    // most recently inserted box was removed.
    virtual Box* getRecent() const;

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

    std::map<int, BoxItem> contents;
    Box* recent;
    BoxDistrib distribution;
    int returnHeight;
    int returnWidth;
    bool dynamicSized;

    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) = 0;

    //--------------------------------------------------------------------------
    // Get the content boundary of the BoxContainer (accounting for horizontal
    // and vertical border sizes) given the position of the BoxContainer.
    Boundary getContentBound(const Position& pos) const;

    //--------------------------------------------------------------------------
    // Get the spacing vector for content printed vertically given the
    // content boundary, total height of the contents, and dynamic box count.
    std::vector<int> getSpacingHeight(const Boundary& container,
        int totalHeight, int dynamCount) const;

    //--------------------------------------------------------------------------
    // Clear the BoxContainer contents and free all allocated memory.
    void clearContents();

};

}
