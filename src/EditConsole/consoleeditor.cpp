//------------------------------------------------------------------------------
// editconsole.cpp
// Implementation for the ConsoleEditor class
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
//------------------------------------------------------------------------------

#include "consoleeditor.h"

const HANDLE ConsoleEditor::OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE ConsoleEditor::IN_HANDLE = GetStdHandle(STD_INPUT_HANDLE);
const HWND ConsoleEditor::WINDOW_HANDLE = GetConsoleWindow();

ConsoleEditor ConsoleEditor::consoleInstance;

//------------------------------------------------------------------------------
ConsoleEditor::ConsoleEditor() : 
    initialized{ false },
    restoreMode{ 0 } {

    initialize();
    formatWriteBuffer();
}

//------------------------------------------------------------------------------
ConsoleEditor& ConsoleEditor::getInstance() {
    return consoleInstance;
}

//------------------------------------------------------------------------------
void ConsoleEditor::initialize() {
    // Save previous mode
    GetConsoleMode(IN_HANDLE, &restoreMode);

    // Enable window input and mouse input in console, and disable quick edit
    // mode.
    DWORD mode = ENABLE_WINDOW_INPUT
        | ENABLE_MOUSE_INPUT
        | ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(IN_HANDLE, mode);
    initialized = true;

    // Clear the console input buffer
    FlushConsoleInputBuffer(IN_HANDLE);
}

//------------------------------------------------------------------------------
void ConsoleEditor::restore() {
    if (!initialized) {
        return;
    }

    // Restore console mode
    SetConsoleMode(IN_HANDLE, restoreMode);
    initialized = false;
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setBufferDimensions(short width, short height) {
    return SetConsoleScreenBufferSize(OUT_HANDLE, COORD{ width, height });
}

//------------------------------------------------------------------------------
bool ConsoleEditor::fitBufferToWindow() {
    Position winSize = getWindowDimensions();
    return setBufferDimensions(winSize.col + 1, winSize.row + 1);
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setWindowDimensions(short width, short height) {
    // Check if screen buffer is too small, resize if necessary
    Position buffSize = getBufferDimensions();
    if (buffSize.col <= width || buffSize.row <= height) {
        setBufferDimensions(width + 1, height + 1);
    }

    SMALL_RECT dim = SMALL_RECT{ 0, 0, width, height };
    bool result = SetConsoleWindowInfo(OUT_HANDLE, TRUE, &dim);
    formatWriteBuffer();
    return result;
}

//------------------------------------------------------------------------------
Position ConsoleEditor::getBufferDimensions() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return Position{ -1, -1 };
    }

    return Position{ winInfo.dwMaximumWindowSize.X,
            winInfo.dwMaximumWindowSize.Y };
}

//------------------------------------------------------------------------------
Position ConsoleEditor::getWindowDimensions() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return Position{ -1, -1 };
    }

    return Position{ winInfo.srWindow.Right, winInfo.srWindow.Bottom };
}

//------------------------------------------------------------------------------
Boundary ConsoleEditor::getBufferBoundary() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return Boundary{ -1, -1, -1, -1 };
    }

    return Boundary{ 0, 0, winInfo.dwMaximumWindowSize.X, 
            winInfo.dwMaximumWindowSize.Y };
}

//------------------------------------------------------------------------------
Boundary ConsoleEditor::getWindowBoundary() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return Boundary{ -1, -1, -1, -1 };
    }

    return Boundary{ 0, 0, winInfo.srWindow.Right, winInfo.srWindow.Bottom };
}

//------------------------------------------------------------------------------
int ConsoleEditor::getWindowWidth() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return -1;
    }

    return winInfo.srWindow.Right + 1;
}

//------------------------------------------------------------------------------
int ConsoleEditor::getWindowHeight() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return -1;
    }

    return winInfo.srWindow.Bottom + 1;
}

//------------------------------------------------------------------------------
InputEvent ConsoleEditor::getButtonInput() {
    INPUT_RECORD inBuff[1];
    int buffSize = readInputBuffer(inBuff, 1);

    // Failed to read console input
    if (buffSize == -1) {
        return InputEvent(inputEvent::Type::INVALID);
    }

    // Ignore input if it's a MOUSE_EVENT signifying only a position change.
    while (inBuff[0].EventType == MOUSE_EVENT
        && inBuff[0].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
        readInputBuffer(inBuff, 1);

        // Failed to read console input
        if (buffSize == -1) {
            return InputEvent(inputEvent::Type::INVALID);
        }
    }

    return InputEvent(inBuff[0]);
}

//------------------------------------------------------------------------------
InputEvent ConsoleEditor::getRawInput() {
    INPUT_RECORD inBuff[1];
    int buffSize = readInputBuffer(inBuff, 1);

    // Failed to read console input
    if (buffSize == -1) {
        return InputEvent(inputEvent::Type::INVALID);
    }

    return InputEvent(inBuff[0]);
}

//------------------------------------------------------------------------------
Position ConsoleEditor::getMousePosition() {
    INPUT_RECORD inBuff[128];
    int buffSize = readInputBuffer(inBuff, 128);

    // Failed to read console input
    if (buffSize == -1) {
        return Position{ -1, -1 };
    }

    for (int i = 0; i < buffSize; ++i) {
        // Check if MOUSE_EVENT
        if (inBuff[i].EventType != MOUSE_EVENT) {
            continue;
        }

        // Return mouse position
        return Position{ inBuff[i].Event.MouseEvent.dwMousePosition.X, 
                inBuff[i].Event.MouseEvent.dwMousePosition.Y };
    }
}

//------------------------------------------------------------------------------
int ConsoleEditor::getMouseX() {
    return getMousePosition().col;
}

//------------------------------------------------------------------------------
int ConsoleEditor::getMouseY() {
    return getMousePosition().row;
}

//------------------------------------------------------------------------------
Position ConsoleEditor::getCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        // Failed to get screen buffer info
        return Position{ -1, -1 };
    }

    return Position{ winInfo.dwCursorPosition.X, winInfo.dwCursorPosition.Y };
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setCursorPosition(Position pos) {
    Position dimensions = getWindowDimensions();

    // Position out-of-bounds
    if (pos.col > dimensions.col || pos.row > dimensions.row 
            || pos.col < 0 || pos.row < 0) {
        return false;
    }

    return SetConsoleCursorPosition(OUT_HANDLE, 
            COORD{ static_cast<short>(pos.col), static_cast<short>(pos.row) });
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setCursorVisibility(bool visible) {
    // Retrieve current cursor info
    CONSOLE_CURSOR_INFO cursorInfo;
    if (!GetConsoleCursorInfo(OUT_HANDLE, &cursorInfo)) {
        // Could not obtain cursor info
        return false;
    }

    cursorInfo.bVisible = visible;
    return SetConsoleCursorInfo(OUT_HANDLE, &cursorInfo);
}

//------------------------------------------------------------------------------
bool ConsoleEditor::resetScrollPosition() {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    SMALL_RECT pos;

    // Get position and dimensions of the console window
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return false;
    }

    // Set position to the top of the window
    pos = winInfo.srWindow;
    pos.Bottom -= pos.Top;
    pos.Top = 0;

    return SetConsoleWindowInfo(OUT_HANDLE, true, &pos);
}

//------------------------------------------------------------------------------
bool ConsoleEditor::scrollWindow(int amount) {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    SMALL_RECT pos;

    // A zero amount does nothing
    if (amount == 0) {
        return true;
    }

    // Get position and dimensions of the console window
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return false;
    }

    pos = winInfo.srWindow;

    // Positive amount; scroll up
    if (amount > 0) {
        // Do not scroll past the window boundary
        if (pos.Top - amount < 0) {
            amount = pos.Top;
        }

        // Set scrolled position
        pos.Top -= amount;
        pos.Bottom -= amount;
    }
    // Negative amount; scroll down
    // ERROR: Need to check for lower boundary
    else {
        // Do not scroll below the window boundary
        if (pos.Bottom - amount >= winInfo.dwSize.Y) {
            amount = winInfo.dwSize.Y - pos.Bottom;
        }
        pos.Top -= amount;
        pos.Bottom -= amount;
    }

    return SetConsoleWindowInfo(OUT_HANDLE, true, &pos);
}

//------------------------------------------------------------------------------
void ConsoleEditor::writeToScreen(const Position& pos, const char text[]) {
    Position prevPos = getCursorPosition();
    setCursorPosition(pos);
    std::cout << text;
    setCursorPosition(prevPos);
}

//------------------------------------------------------------------------------
void ConsoleEditor::writeToBuffer(const Position& pos, const char text[]) {
    int textIdx = 0;
    if (writeBuffer.size() == 0) {
        return;
    }
    Position drawIdx = { pos.col, pos.row };
    Position buffDim{ writeBuffer[0].size(), writeBuffer.size() - 1 };

    // Check draw position
    if (pos.col < 0 || pos.col > buffDim.col - 1) {
        return;
    }
    if (pos.row < 0 || pos.row > buffDim.row - 1) {
        return;
    }

    // Write to write buffer
    while (text[textIdx] && writeBuffer[pos.row][drawIdx.col]) {
        writeBuffer[pos.row][drawIdx.col++] = text[textIdx++];
    }
}

//------------------------------------------------------------------------------
void ConsoleEditor::printWriteBuffer() {
    Position prevPos = getCursorPosition();
    Position writePos = { 0, 0 };

    for (writePos.row; writePos.row < writeBuffer.size(); ++writePos.row) {
        setCursorPosition(writePos);
        std::cout << &writeBuffer[writePos.row][0];
    }

    setCursorPosition(prevPos);
}

//------------------------------------------------------------------------------
// https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
void ConsoleEditor::clearScreen() {
    COORD tl = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(OUT_HANDLE, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(OUT_HANDLE, ' ', cells, tl, &written);
    FillConsoleOutputAttribute(OUT_HANDLE, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(OUT_HANDLE, tl);
}

//------------------------------------------------------------------------------
void ConsoleEditor::clearWriteBuffer() {
    int rows = writeBuffer.size();
    int cols = writeBuffer[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            writeBuffer[i][j] = ' ';
        }
        writeBuffer[i][cols - 1] = '\0';
    }
}

//------------------------------------------------------------------------------
void ConsoleEditor::clearInputBuffer() {
    FlushConsoleInputBuffer(IN_HANDLE);
}

//------------------------------------------------------------------------------
void ConsoleEditor::formatWriteBuffer() {
    Position winDim = getWindowDimensions();
    writeBuffer = std::vector<std::vector<char>>(winDim.row + 1, 
            std::vector<char>(winDim.col + 1));
    clearWriteBuffer();
}

//------------------------------------------------------------------------------
int ConsoleEditor::readInputBuffer(INPUT_RECORD inBuff[], int buffSize) {
    DWORD readRecords;
    if (!ReadConsoleInput(IN_HANDLE, inBuff, buffSize, &readRecords)) {
        // Failed to read console input
        return -1;
    }

    return readRecords;
}