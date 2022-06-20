//------------------------------------------------------------------------------
// horizcontainer.h
// Interface for the HorizContainer class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A HorizContainer is a type of Box that contains and manages
//     other Box objects. HorizContainers are responsible for printing all
//     contained Boxes horizontally within its boundary according to the
//     specified Alignment and BoxDistrib flags applied. Any input MouseEvent is
//     passed into the corresponding contained Box and returns its Reply
//
// Class Functionality:
//     - Inherits all methods and functionality of the BoxContainer class.
//     - Implements the draw() and interact() BoxContainer methods.
//     - Draw the HorizContainer to the console window.
// 
// Dependencies: BoxContainer class.
//------------------------------------------------------------------------------

#include "Box/BoxContainer/boxcontainer.h"

class HorizContainer : public BoxContainer {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    HorizContainer();

    //--------------------------------------------------------------------------
    // Parameterized constructor
    HorizContainer(int width, int height);

    //--------------------------------------------------------------------------
    // Copy constructor
    HorizContainer(const HorizContainer& cpy);

    //--------------------------------------------------------------------------
    // Draw the contents of the BoxContainer to the output console given an
    // origin column and row, and a constraining rectangle that represents the
    // container boundaries that the BoxContainer is within.
    virtual Reply draw(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Buffer the contents BoxContainer to EditConsole's write buffer given an
    // origin column and row, and a constraining rectangle that represents the
    // container boundaries that the BoxContainer is within.
    virtual Reply buffer(Position pos, Boundary container) override;

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
    // Get the spacing vector for content printed horizontally given the
    // content boundary, total width of the contents, and dynamic box count.
    std::vector<int> getSpacingWidth(const Boundary& container,
        int totalWidth, int dynamCount) const;

    //--------------------------------------------------------------------------
    // Return the calculated row offset of a Box given the Box's height. The
    // row offset is the the verticle offset of the Box position relative to 
    // the origin of the BoxContainer.
    // Assumes that the actual dimensions of the BoxContainer have been
    // calculated.
    int getRowOffset(int boxHeight) const;

};