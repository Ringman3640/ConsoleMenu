//------------------------------------------------------------------------------
// livetextbox.h
// Interface for the LiveTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A LiveTextBox is a TextBox that stores a variable reference to 
//     display to the screen. This reference relationship means that any changes
//     made to the variable are automatically applied to the LiveTextBox when
//     drawing or buffering. LiveTextBoxes support multiple basic data types.
//
// Class Functionality:
//     - Inherits all methods and functionality of the TextBox class.
//     - Draw the LiveTextBox to the console window.
//     - Set the reference variable of the LiveTextBox.
// 
// Supported Data Types:
//     - int
//     - long
//     - unsigned
//     - float
//     - double
//     - std::string
// 
// Dependencies: TextBox class.
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include "Box/ContentBox/TextBox/textbox.h"

namespace conu {

//------------------------------------------------------------------------------
class LiveTextBox : public TextBox {
public:
    //--------------------------------------------------------------------------
    // Default constructor
    LiveTextBox(std::string text = std::string());

    //--------------------------------------------------------------------------
    // Parameterized constructor
    LiveTextBox(int width, int height, std::string text = std::string());

    //--------------------------------------------------------------------------
    // Execute an action given a specific mouse event. Returns an IGNORED Reply.
    virtual Reply interact(inputEvent::MouseEvent action) override;

    //--------------------------------------------------------------------------
    // Create a deep copy of this LiveTextBox object and return a pointer to 
    // that copy.
    virtual Box* copyBox() const override;

    //--------------------------------------------------------------------------
    // Create a new v object and return a pointer to that object.
    virtual Box* createBox() const override;

    //--------------------------------------------------------------------------
    // Get the name of the specific LiveTextBox class. Returns "TextBox".
    virtual std::string getClassName() const override;

    //--------------------------------------------------------------------------
    // Set the reference varaible used by the LiveTextBox when drawn.
    // Function overloads for all supported data types.
    void setLiveVariable(int& var);
    void setLiveVariable(long& var);
    void setLiveVariable(unsigned& var);
    void setLiveVariable(float& var);
    void setLiveVariable(double& var);
    void setLiveVariable(std::string& var);

    // Deleted setText() method
    void setText(std::string text) = delete;

private:
    // VARAIBLE_TYPE enumerator
    // Indicates which type of pointer is saved in the savedVar union
    enum class VARIABLE_TYPE {
        INT,
        LONG,
        UNSIGNED,
        FLOAT,
        DOUBLE,
        STRING,
        UNINITIALIZED
    };

    // LiveVariableData union
    // Union holding all supported data types of the LiveTextBox as pointers.
    union LiveVariableData {
        int* intPtr;
        long* longPtr;
        unsigned* unsignedPtr;
        float* floatPtr;
        double* doublePtr;
        std::string* stringPtr;

        // Uninitialized constructor
        LiveVariableData();
    };

    LiveVariableData savedVar;
    VARIABLE_TYPE savedType;

    //--------------------------------------------------------------------------
    // The protocol used to print the Box object to the screen or buffer
    // (indicated by the drawMode parameter). Each derived class of Box should
    // implement their own protocol, which is then called through draw(),
    // buffer(), redraw(), or rebuffer().
    virtual Reply printProtocol(Position pos, Boundary container,
            bool drawMode) override;

    //--------------------------------------------------------------------------
    // Update the displayed contents of the LiveTextBox.
    // Helper function for printProtocol().
    void updateTextBoxContent();

};

}