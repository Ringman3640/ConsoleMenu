//------------------------------------------------------------------------------
// button.h
// Interface for the Button class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A Button is a type of TextBox that performs some user-defined
//     execution when clicked on. 
//
// Class Functionality:
//     - Inherits all methods and functionality of the TextBox class.
//     - Draw the Button to the console window.
//     - Set the text of the Button that will be printed.
//     - Interact with the Button with a left click, which performs a 
//       user-defined execution.
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#include <functional>
#include "Box/ContentBox/TextBox/textbox.h"

namespace conu {

class Button : public TextBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    Button(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructor
    Button(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Execute an action given a mouse click. Returns IGNORED if the mouse
    // event is not a left click. Otherwise returns a user-specified Reply.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this Button object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new Button object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific Button class. Returns "Button".
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Set the executing function that is called when the Button is Clicked.
    // The function has a parameter to a Box reference. When the function is 
    // invoked, the current Button will pass a reference to itself as an
    // argument.
    void setAction(std::function<Reply(Box&)> action);

private:
    //--------------------------------------------------------------------------
    // Click handler function that is called in interact()
    std::function<Reply(Box&)> clickHandler;

    //--------------------------------------------------------------------------
    // Static method for default clickHandler value
    static Reply DEFAULT_CLICK_HANDLER(Box& self);

};

}
