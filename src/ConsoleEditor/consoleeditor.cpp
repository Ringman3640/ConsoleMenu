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
// Supported OS: Windows
//------------------------------------------------------------------------------

#include "consoleeditor.h"

namespace conu {

const HANDLE ConsoleEditor::OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE ConsoleEditor::IN_HANDLE = GetStdHandle(STD_INPUT_HANDLE);
const HWND ConsoleEditor::WINDOW_HANDLE = GetConsoleWindow();

ConsoleEditor ConsoleEditor::consoleInstance;

//------------------------------------------------------------------------------
ConsoleEditor::ConsoleEditor() :
    initialized{ false },
    restoreMode{ 0 },
    resizeManagerThread{ },
    resizeHandler{ []() { return; } },
    terminateResizeManager{ false },
    resizeManagerActive{ false } {

    formatWriteBuffer();
}

//------------------------------------------------------------------------------
ConsoleEditor::~ConsoleEditor() {
    stopResizeManager();
}

//------------------------------------------------------------------------------
ConsoleEditor& ConsoleEditor::getInstance() {
    return consoleInstance;
}

//------------------------------------------------------------------------------
void ConsoleEditor::initialize() {
    GetConsoleMode(IN_HANDLE, &restoreMode);

    // Enable window input and mouse input in console, and disable quick edit
    // mode.
    DWORD mode = ENABLE_WINDOW_INPUT
        | ENABLE_MOUSE_INPUT
        | ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(IN_HANDLE, mode);
    initialized = true;

    FlushConsoleInputBuffer(IN_HANDLE);
}

//------------------------------------------------------------------------------
void ConsoleEditor::restore() {
    if (!initialized) {
        return;
    }

    SetConsoleMode(IN_HANDLE, restoreMode);
    initialized = false;

    stopResizeManager();
}

//------------------------------------------------------------------------------
void ConsoleEditor::startResizeManager() {
    if (resizeManagerActive) {
        return;
    }

    std::lock_guard<std::mutex> lock(writeBufferLock);
    terminateResizeManager = false;
    resizeManagerActive = true;
    resizeManagerThread = std::thread(&ConsoleEditor::resizeManager, this);
}

//------------------------------------------------------------------------------
void ConsoleEditor::stopResizeManager() {
    if (!resizeManagerActive) {
        return;
    }

    std::lock_guard<std::mutex> lock(writeBufferLock);
    terminateResizeManager = true;
    resizeManagerThread.join();
    resizeManagerActive = false;
}

//------------------------------------------------------------------------------
bool ConsoleEditor::resizeManagerRunning() const {
    return resizeManagerActive;
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setWindowDimensions(short width, short height) {
    // Check if screen buffer is too small, resize if necessary
    CONSOLE_SCREEN_BUFFER_INFO buffSize;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &buffSize)) {
        return false;
    }
    if (buffSize.dwMaximumWindowSize.X <= width 
            || buffSize.dwMaximumWindowSize.Y <= height) {
        SHORT buffWidth = std::max<int>(width, buffSize.dwMaximumWindowSize.X);
        SHORT buffHeight = std::max<int>(height, buffSize.dwMaximumWindowSize.Y);
        SetConsoleScreenBufferSize(OUT_HANDLE, COORD{ buffWidth, buffHeight });
    }

    SMALL_RECT dim = SMALL_RECT{ 0, 0, width - 1, height - 1 };
    if (!SetConsoleWindowInfo(OUT_HANDLE, TRUE, &dim)) {
        return false;
    }
    formatWriteBuffer();
    return fitBufferToWindow();
}

//------------------------------------------------------------------------------
void ConsoleEditor::allowWindowResizing(bool resizable) {
    HWND handle = GetConsoleWindow();
    if (resizable) {
        SetWindowLong(handle, GWL_STYLE, GetWindowLong(handle, GWL_STYLE)
            | WS_SIZEBOX);
    }
    else {
        SetWindowLong(handle, GWL_STYLE, GetWindowLong(handle, GWL_STYLE)
            & ~WS_SIZEBOX);
    }
}

//------------------------------------------------------------------------------
void ConsoleEditor::allowMaximizeBox(bool maximizable) {
    HWND handle = GetConsoleWindow();
    if (maximizable) {
        SetWindowLong(handle, GWL_STYLE, GetWindowLong(handle, GWL_STYLE)
            | WS_MAXIMIZEBOX);
    }
    else {
        SetWindowLong(handle, GWL_STYLE, GetWindowLong(handle, GWL_STYLE)
            & ~WS_MAXIMIZEBOX);
    }
}

//------------------------------------------------------------------------------
Position ConsoleEditor::getWindowDimensions() const {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return Position{ -1, -1 };
    }

    return Position{ winInfo.srWindow.Right + 1, winInfo.srWindow.Bottom + 1 };
}

//------------------------------------------------------------------------------
Boundary ConsoleEditor::getWindowBoundary() const {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return Boundary{ -1, -1, -1, -1 };
    }

    return Boundary{ 0, 0, winInfo.srWindow.Right, winInfo.srWindow.Bottom };
}

//------------------------------------------------------------------------------
int ConsoleEditor::getWindowWidth() const {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return -1;
    }

    return winInfo.srWindow.Right + 1;
}

//------------------------------------------------------------------------------
int ConsoleEditor::getWindowHeight() const {
    CONSOLE_SCREEN_BUFFER_INFO winInfo;
    if (!GetConsoleScreenBufferInfo(OUT_HANDLE, &winInfo)) {
        return -1;
    }

    return winInfo.srWindow.Bottom + 1;
}

//------------------------------------------------------------------------------
InputEvent ConsoleEditor::getButtonInput() {
    INPUT_RECORD inBuff[1];
    int buffSize = readInputBuffer(inBuff, 1);

    if (buffSize == -1) {
        return InputEvent(inputEvent::Type::INVALID);
    }

    // Ignore input if it's a MOUSE_EVENT signifying only a position change.
    while (inBuff[0].EventType == MOUSE_EVENT
        && inBuff[0].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
        readInputBuffer(inBuff, 1);

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
    CONSOLE_CURSOR_INFO cursorInfo;
    if (!GetConsoleCursorInfo(OUT_HANDLE, &cursorInfo)) {
        return false;
    }

    cursorInfo.bVisible = visible;
    return SetConsoleCursorInfo(OUT_HANDLE, &cursorInfo);
}

//------------------------------------------------------------------------------
bool ConsoleEditor::setFontSize(int size) {
    CONSOLE_FONT_INFOEX fontInfo;

    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (!GetCurrentConsoleFontEx(OUT_HANDLE, FALSE, &fontInfo)) {
        return false;
    }

    fontInfo.dwFontSize.Y = size;
    fontInfo.dwFontSize.X = size / 2;

    return SetCurrentConsoleFontEx(OUT_HANDLE, FALSE, &fontInfo);
}

//------------------------------------------------------------------------------
void ConsoleEditor::writeToScreen(const Position& pos, const char text[]) {
    Position prevPos = getCursorPosition();
    LPDWORD charsWritten = 0;
    DWORD charsToWrite = 0;

    for (charsToWrite = 0; text[charsToWrite]; ++charsToWrite);

    setCursorPosition(pos);
    WriteConsoleA(OUT_HANDLE, text, charsToWrite, charsWritten, NULL);
    setCursorPosition(prevPos);
}

//------------------------------------------------------------------------------
void ConsoleEditor::writeToBuffer(const Position& pos, const char text[]) {
    std::lock_guard<std::mutex> lock(writeBufferLock);

    int textIdx = 0;
    if (writeBuffer.size() == 0) {
        return;
    }
    Position drawIdx = { pos.col, pos.row };
    Position buffDim{ writeBuffer[0].size() - 1, writeBuffer.size() - 1 };

    // Check draw position
    if (pos.col < 0 || pos.col > buffDim.col) {
        return;
    }
    if (pos.row < 0 || pos.row > buffDim.row) {
        return;
    }

    // Write to write buffer
    while (text[textIdx] && pos.col + textIdx < writeBuffer[pos.row].size()) {
        writeBuffer[pos.row][drawIdx.col++] = text[textIdx++];
    }
}

//------------------------------------------------------------------------------
void ConsoleEditor::printWriteBuffer() {
    std::lock_guard<std::mutex> lock(writeBufferLock);

    Position prevPos = getCursorPosition();
    Position writePos = { 0, 0 };
    LPDWORD charsWritten = 0;

    for (writePos.row; writePos.row < writeBuffer.size(); ++writePos.row) {
        setCursorPosition(writePos);
        WriteConsoleA(OUT_HANDLE, &writeBuffer[writePos.row][0], 
                writeBuffer[writePos.row].size(), charsWritten, NULL);
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
    std::lock_guard<std::mutex> lock(writeBufferLock);

    int rows = writeBuffer.size();
    int cols = writeBuffer[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            writeBuffer[i][j] = ' ';
        }
    }
}

//------------------------------------------------------------------------------
void ConsoleEditor::clearInputBuffer() {
    FlushConsoleInputBuffer(IN_HANDLE);
}

//------------------------------------------------------------------------------
void ConsoleEditor::formatWriteBuffer() {
    std::unique_lock<std::mutex> lock(writeBufferLock);

    Position winDim = getWindowDimensions();
    writeBuffer = std::vector<std::vector<char>>(winDim.row,
        std::vector<char>(winDim.col));

    lock.unlock();
    clearWriteBuffer();
}

//------------------------------------------------------------------------------
void ConsoleEditor::setResizeHandler(std::function<void(void)> resizeHandler) {
    this->resizeHandler = resizeHandler;
}

//------------------------------------------------------------------------------
int ConsoleEditor::readInputBuffer(INPUT_RECORD inBuff[], int buffSize) {
    DWORD readRecords;
    if (!ReadConsoleInput(IN_HANDLE, inBuff, buffSize, &readRecords)) {
        return -1;
    }

    return readRecords;
}

//------------------------------------------------------------------------------
bool ConsoleEditor::fitBufferToWindow() {
    Position winSize = getWindowDimensions();
    SMALL_RECT dim = { 0, 0, winSize.col - 1, winSize.row - 1 };

    // I do not know why, but I need to resize the window buffer screen and the
    // window buffer to accurately resize the screen (removing the additional
    // width and height from the scroll bars)
    // Thank you win32 api
    if (!SetConsoleScreenBufferSize(OUT_HANDLE, 
            COORD{ static_cast<short>(winSize.col), 
            static_cast<short>(winSize.row) })) {
        return false;
    }

    if (!SetConsoleWindowInfo(OUT_HANDLE, TRUE, &dim)) {
        return false;
    }
    return SetConsoleScreenBufferSize(OUT_HANDLE,
            COORD{ static_cast<short>(winSize.col),
            static_cast<short>(winSize.row) });
}

//------------------------------------------------------------------------------
void ConsoleEditor::resizeManager() {
    Position prevDim = getWindowDimensions(), currDim;

    while (true) {
        if (terminateResizeManager) {
            return;
        }

        currDim = getWindowDimensions();
        if (currDim.col != prevDim.col || currDim.row != prevDim.row) {
            formatWriteBuffer();
            fitBufferToWindow();
            resizeHandler();
            prevDim = currDim;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

}