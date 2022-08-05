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
    // Draw the LiveTextBox to the output console given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the LiveTextBox is within. Returns a CONTINUE Reply.
    virtual Reply draw(Position pos, Boundary container) override;

    //--------------------------------------------------------------------------
    // Buffer the Box to EditConsole's write buffer given an origin column and
    // row, and a constraining rectangle that represents the container
    // boundaries that the Box is within. 
    virtual Reply buffer(Position pos, Boundary container) override;

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

    void updateTextBoxContent();

};

}