//------------------------------------------------------------------------------
// livetextbox.cpp
// Implementation for the LiveTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: A LiveTextBox is a TextBox that stores a variable reference to 
//     display to the screen. This reference relationship means that any changes
//     made to the variable are automatically applied to the LiveTextBox when
//     drawing or buffering. LiveTextBoxes support multiple basic data types.
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

#include "Box/ContentBox/TextBox/livetextbox.h"

namespace conu {

//------------------------------------------------------------------------------
LiveTextBox::LiveVariableData::LiveVariableData() :
    intPtr{ nullptr } {

}

//------------------------------------------------------------------------------
LiveTextBox::LiveTextBox(std::string text) :
    TextBox(text),
    savedVar{ },
    savedType{ VARIABLE_TYPE::UNINITIALIZED } {

}

//------------------------------------------------------------------------------
LiveTextBox::LiveTextBox(int width, int height, std::string text) :
    TextBox(width, height, text),
    savedVar{ },
    savedType{ VARIABLE_TYPE::UNINITIALIZED } {

}

//------------------------------------------------------------------------------
Reply LiveTextBox::interact(inputEvent::MouseEvent action) {
    return Reply::IGNORED;
}

//------------------------------------------------------------------------------
Box* LiveTextBox::copyBox() const {
    return new LiveTextBox(*this);
}

//------------------------------------------------------------------------------
Box* LiveTextBox::createBox() const {
    return new LiveTextBox();
}

//------------------------------------------------------------------------------
std::string LiveTextBox::getClassName() const {
    return std::string("LiveTextBox");
}

//------------------------------------------------------------------------------
Reply LiveTextBox::printProtocol(Position pos, Boundary container,
        bool drawMode) {
    updateTextBoxContent();
    return TextBox::printProtocol(pos, container, drawMode);
}

//------------------------------------------------------------------------------
void LiveTextBox::setLiveVariable(int& var) {
    savedVar.intPtr = &var;
    savedType = VARIABLE_TYPE::INT;
}
void LiveTextBox::setLiveVariable(long& var) {
    savedVar.longPtr = &var;
    savedType = VARIABLE_TYPE::LONG;
}
void LiveTextBox::setLiveVariable(unsigned& var) {
    savedVar.unsignedPtr = &var;
    savedType = VARIABLE_TYPE::UNSIGNED;
}
void LiveTextBox::setLiveVariable(float& var) {
    savedVar.floatPtr = &var;
    savedType = VARIABLE_TYPE::FLOAT;
}
void LiveTextBox::setLiveVariable(double& var) {
    savedVar.doublePtr = &var;
    savedType = VARIABLE_TYPE::DOUBLE;
}
void LiveTextBox::setLiveVariable(char& var) {
    savedVar.charPtr = &var;
    savedType = VARIABLE_TYPE::CHAR;
}
void LiveTextBox::setLiveVariable(std::string& var) {
    savedVar.stringPtr = &var;
    savedType = VARIABLE_TYPE::STRING;
}

//------------------------------------------------------------------------------
void LiveTextBox::updateTextBoxContent() {
    std::string content;

    switch (savedType) {
    case VARIABLE_TYPE::INT:
        content = std::to_string(*savedVar.intPtr);
        break;

    case VARIABLE_TYPE::LONG:
        content = std::to_string(*savedVar.longPtr);
        break;

    case VARIABLE_TYPE::UNSIGNED:
        content = std::to_string(*savedVar.unsignedPtr);
        break;

    case VARIABLE_TYPE::FLOAT:
        content = std::to_string(*savedVar.floatPtr);
        break;

    case VARIABLE_TYPE::DOUBLE:
        content = std::to_string(*savedVar.doublePtr);
        break;

    case VARIABLE_TYPE::CHAR:
        content = *savedVar.charPtr;
        break;

    case VARIABLE_TYPE::STRING:
        content = *savedVar.stringPtr;
        break;

    case VARIABLE_TYPE::UNINITIALIZED:
    default:
        break;
    }
    
    //setText(content);
    text = content;
}

}