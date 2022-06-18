//------------------------------------------------------------------------------
// editconsole.h
// Interface for the ConsoleEditor class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The ConsoleEditor class provides functions to view, modify,
//      and control data regarding the default program console window. Acts as
//      a wrapper for Windows API console functions. This class is implemented
//      as a singleton; an instance must be aquired through the
//      ConsoleEditor::getInstance() method. 
//
// Class Functionality:
//     - Get and set the console window dimensions in character units.
//     - Get and set the console buffer dimensions in character units.
//     - Receive input from the console's input buffer.
//     - Get the position of the mouse cursor on the console window.
//     - Get and set the position of the console cursor on the console window. 
//     - Scroll the console window vertically a specified number of character
//       units.
//     - Clear the console screen.
//     - Clear the input buffer.
// 
// Supported OS: Windows
// Dependencies: InputEvent struct
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConsoleEditor/inputevent.h"

//------------------------------------------------------------------------------
class ConsoleEditor {

public:
    //--------------------------------------------------------------------------
    // Get an instance of the singleton ConsoleEditor object.
    static ConsoleEditor& getInstance();

    //--------------------------------------------------------------------------
    // Change console mode settings that are necessary for function use. 
    void initialize();

    //--------------------------------------------------------------------------
    // Return console mode settings to their original options when initialize()
    // was called.
    void restore();

    //--------------------------------------------------------------------------
    // Resize the console screen buffer to fit the size of the console window.
    bool fitBufferToWindow();

    //--------------------------------------------------------------------------
    // Set the height and width of the console screen buffer in character units.
    bool setBufferDimensions(short width, short height);

    //--------------------------------------------------------------------------
    // Set the height and width of the active console window in chracter units. 
    bool setWindowDimensions(short width, short height);

    //--------------------------------------------------------------------------
    // Get the height and width of the console screen buffer in character units.
    Position getBufferDimensions();

    //--------------------------------------------------------------------------
    // Get the height and width of the active console window in character units.
    Position getWindowDimensions();

    //--------------------------------------------------------------------------
    // Get the dimensions of the console screen buffer as a Boundary struct.
    Boundary getBufferBoundary();

    //--------------------------------------------------------------------------
    // Get the dimensions of the active console window as a Boundary struct.
    Boundary getWindowBoundary();

    //--------------------------------------------------------------------------
    // Get the width of the active console window in character units.
    int getWindowWidth();

    //--------------------------------------------------------------------------
    // Get the width of the active console window in character units.
    int getWindowHeight();

    //--------------------------------------------------------------------------
    // Get a mouse or keyboard input from the console input buffer.
    InputEvent getButtonInput();

    //--------------------------------------------------------------------------
    // Get any input from in the console input buffer, inluding mouse movements.
    InputEvent getRawInput();

    //--------------------------------------------------------------------------
    // Get the current X and Y position of the mouse cursor.
    Position getMousePosition();

    //--------------------------------------------------------------------------
    // Get the current X position of the mouse cursor.
    int getMouseX();

    //--------------------------------------------------------------------------
    // Get the current Y position of the mouse cursor.
    int getMouseY();

    //--------------------------------------------------------------------------
    // Get the current X and Y position of the console cursor.
    Position getCursorPosition();

    //--------------------------------------------------------------------------
    // Set the X and Y position of the console cursor. 
    bool setCursorPosition(Position pos);

    //--------------------------------------------------------------------------
    // Set whether or not the console cursor is visible on the console window. 
    bool setCursorVisibility(bool visible);

    //--------------------------------------------------------------------------
    // Reset the scroll position to show the top of the console window.
    bool resetScrollPosition();

    //--------------------------------------------------------------------------
    // Scroll the console window a given amount of rows. A positive amount
    // scrolls the window up. A negative amount scrolls the window down.
    bool scrollWindow(int amount);

    //--------------------------------------------------------------------------
    // Write character text to the console screen starting at some given
    // position.
    void writeToScreen(const Position& pos, const char text[]);

    //--------------------------------------------------------------------------
    // Add character text to the write buffer starting at some given position.
    void writeToBuffer(const Position& pos, const char text[]);

    //--------------------------------------------------------------------------
    // Print the contents of the write buffer to the console window.
    void printWriteBuffer();

    //--------------------------------------------------------------------------
    // Clear the console screen.
    void clearScreen();

    //--------------------------------------------------------------------------
    // Clear the write buffer with space characters.
    void clearWriteBuffer();

    //--------------------------------------------------------------------------
    // Clear the input buffer.
    void clearInputBuffer();

    //--------------------------------------------------------------------------
    // Format the write buffer to the size of the current console window.
    void formatWriteBuffer();

private:
    //--------------------------------------------------------------------------
    // Static handlers
    static const HANDLE OUT_HANDLE;
    static const HANDLE IN_HANDLE;
    static const HWND WINDOW_HANDLE;

    //--------------------------------------------------------------------------
    // Static singleton instance
    static ConsoleEditor consoleInstance;

    //--------------------------------------------------------------------------
    // Windows console mode restoration members
    bool initialized;
    DWORD restoreMode;

    //--------------------------------------------------------------------------
    // Write buffer information
    std::vector<std::vector<char>> writeBuffer;

    //--------------------------------------------------------------------------
    // Private default constructor for ConsoleEditor class.
    ConsoleEditor();

    //--------------------------------------------------------------------------
    // Read from the console input buffer into an INPUT_RECORD array.
    int readInputBuffer(INPUT_RECORD inBuff[], int buffSize);

};