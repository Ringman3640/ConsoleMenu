//------------------------------------------------------------------------------
// entrytextbox.h
// Interface for the EntryTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An EntryTextBox is a type of TextBox that allows the user to
//     input a text value when the box is interacted. The EntryTextBox stops
//     reading the user input when the Enter button is clicked or if the user
//     clicks off the box.
// 
// Dependencies: TextBox and MenuManager class.
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <mutex>
#include <functional>
#include "Box/ContentBox/TextBox/textbox.h"
#include "Menu/menumanager.h"

namespace conu {

//------------------------------------------------------------------------------
class EntryTextBox : public TextBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    EntryTextBox(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructor
    EntryTextBox(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Copy constructor
    EntryTextBox(const EntryTextBox& copy);

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event. Returns an IGNORED Reply.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this TextBox object and return a pointer to that 
    // copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new TextBox object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific TextBox class. Returns "EntryTextBox".
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Set the content display text of the EntryTextBox. This text is visible
    // within the EntryTextBox when the user is not interacting with it. If the
    // passed text is an empty string (default), the EntryTextBox will display
    // the current user input.
    void setText(std::string text);

    //--------------------------------------------------------------------------
    // Set the handler routine that is called when the user inputs a key. The
    // handler routine receives a character that was inputted by the user and 
    // returns a boolean indicating if the character should be accepted. If the
    // handler returns false, the character is ignored. 
    void setInputHandler(std::function<bool(char)> handler);

    //--------------------------------------------------------------------------
    // Set the handler routine that is called when the user cancels the entry
    // process. This occurs when the user clicks off the EntryTextBox when
    // previously interacting with it. The handler routine receives a reference
    // string to the current user input.
    void setCancelHandler(std::function<void(std::string&)> handler);

    //--------------------------------------------------------------------------
    // Set the handler routine that is called when the user submits an input to
    // the EntryTextBox. This occurs when the user presses the Entry key while
    // interacting with the EntryTextBox. The handler routine receives a
    // reference string to the current user input.
    void setProcessHandler(std::function<void(std::string&)> handler);

    //--------------------------------------------------------------------------
    // Set the current user input string.
    void setInput(std::string input);

    //--------------------------------------------------------------------------
    // Get the current user input string.
    std::string getInput() const;

    //--------------------------------------------------------------------------
    // Clear the current user input string.
    void clearInput();

    //--------------------------------------------------------------------------
    // Set whether the EntryTextBox will automatically refresh the Menu screen's
    // contents when the EntryTextBox text changes. This is off by default since
    // a Menu will default to using auto print.
    void autoMenuRefreshing(bool autoRefreshing);

protected:
    mutable std::mutex textLock;
    std::string displayText;
    std::string userInput;

    bool userInteracting;
    bool menuRefreshing;

    std::function<bool(char)> inputHandler;
    std::function<void(std::string&)> cancelHandler;
    std::function<void(std::string&)> processHandler;

    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) override;
        
    //--------------------------------------------------------------------------
    // Try to refresh the current Menu based on the state of menuRefreshing.
    // Helper method for interact().
    void tryMenuRefresh() const;
};

}
