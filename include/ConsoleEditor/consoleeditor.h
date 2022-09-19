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
// Supported OS: Windows
// Dependencies: InputEvent struct
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <algorithm>
#include "ConsoleEditor/inputevent.h"

namespace conu {

//------------------------------------------------------------------------------
class ConsoleEditor {

public:
    //--------------------------------------------------------------------------
    // Destructor
    ~ConsoleEditor();

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
    // Check if ConsoleEditor has been initialized.
    bool initialized() const;

    //--------------------------------------------------------------------------
    // Launch the resize manager if it is not already started.
    void startResizeManager();

    //--------------------------------------------------------------------------
    // Terminate the resize manager if it is currently running.
    void stopResizeManager();

    //--------------------------------------------------------------------------
    // Check if the resize manager is currently running.
    bool resizeManagerRunning() const;

    //--------------------------------------------------------------------------
    // Set the height and width of the active console window in chracter units. 
    bool setWindowDimensions(short width, short height);

    //--------------------------------------------------------------------------
    // Set whether or not the console window can be resized from the corner.
    void allowWindowResizing(bool resizable);

    //--------------------------------------------------------------------------
    // Set whether or not the console window can be maximized.
    void allowMaximizeBox(bool maximizable);

    //--------------------------------------------------------------------------
    // Get the height and width of the active console window in character units.
    Position getWindowDimensions() const;

    //--------------------------------------------------------------------------
    // Get the dimensions of the active console window as a Boundary struct.
    Boundary getWindowBoundary() const;

    //--------------------------------------------------------------------------
    // Get the width of the active console window in character units.
    int getWindowWidth() const;

    //--------------------------------------------------------------------------
    // Get the width of the active console window in character units.
    int getWindowHeight() const;

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
    // Set the font size of the console text.
    bool setFontSize(int size);

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

    //--------------------------------------------------------------------------
    // Set the resize handler that is called whenever the resize manager detects
    // a change in dimensions of the console window.
    void setResizeHandler(std::function<void(void)> resizeHandler);

private:
    //--------------------------------------------------------------------------
    // Static handlers
    static const HANDLE OUT_HANDLE;
    static const HANDLE IN_HANDLE;
    static const HWND WINDOW_HANDLE;

    // Static singleton instance
    static ConsoleEditor consoleInstance;

    // Windows console mode restoration members
    bool init;
    DWORD restoreMode;

    // Resize manager thread instance members
    std::thread resizeManagerThread;
    std::function<void(void)> resizeHandler;
    bool terminateResizeManager, resizeManagerActive;

    // Write buffer information
    std::vector<std::vector<char>> writeBuffer;
    std::mutex writeBufferLock;

    //--------------------------------------------------------------------------
    // Private default constructor for ConsoleEditor class.
    ConsoleEditor();

    //--------------------------------------------------------------------------
    // Read from the console input buffer into an INPUT_RECORD array.
    int readInputBuffer(INPUT_RECORD inBuff[], int buffSize);

    //--------------------------------------------------------------------------
    // Resize the console screen buffer to fit the size of the console window.
    bool fitBufferToWindow();

    //--------------------------------------------------------------------------
    // Thread for polling and handling window resizing events.
    void resizeManager();

};

}
