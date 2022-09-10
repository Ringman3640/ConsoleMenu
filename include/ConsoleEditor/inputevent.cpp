//------------------------------------------------------------------------------
// inputevent.cpp
// Implementation for the InputEvent struct
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An InputEvent struct contains information about a buffered 
//     console input. Each InputEvent struct can contain information about a
//     mouse input, keyboard input, or console screen resize input. The type
//     of input contained in the InputEvent is defined by an enumerator value
//     in 'type', which corresponds to a union value 'info'. Simplification
//     of the Windows API INPUT_RECORD struct.
//
// Struct Purpose: The purpose of the InputEvent struct is to simplify and 
//     abstract the information of the Windows INPUT_RECORD struct. The
//     InputEvent struct constructors and initializers can be reimplemented
//     for other operating systems such that reliant systems can still use
//     the InputEvent struct interface.
// 
// Supported OS: Windows
//------------------------------------------------------------------------------

#include "ConsoleEditor/inputevent.h"

namespace conu {

//------------------------------------------------------------------------------
inputEvent::Key inputEvent::operator | (Key left, Key right) {
    return static_cast<Key>(static_cast<std::underlying_type<Key>::type>(left)
        | static_cast<std::underlying_type<Key>::type>(right));
}

//------------------------------------------------------------------------------
int inputEvent::operator & (Key left, Key right) {
    return static_cast<std::underlying_type<Key>::type>(
        static_cast<std::underlying_type<Key>::type>(left)
        & static_cast<std::underlying_type<Key>::type>(right));
}

//------------------------------------------------------------------------------
InputEvent::InputEvent(inputEvent::Type type) :
    type{ type } {

}

//------------------------------------------------------------------------------
InputEvent::InputEvent(INPUT_RECORD inRecord) {
    switch (inRecord.EventType) {
    case MOUSE_EVENT:
        type = inputEvent::Type::MOUSE_INPUT;
        info.mouse = initMouseEvent(inRecord.Event.MouseEvent);
        return;

    case KEY_EVENT:
        type = inputEvent::Type::KEY_INPUT;
        info.key = initKeyEvent(inRecord.Event.KeyEvent);
        return;

    case WINDOW_BUFFER_SIZE_EVENT:
        type = inputEvent::Type::RESIZE_INPUT;
        info.resize = initResizeEvent(inRecord.Event.WindowBufferSizeEvent);
        return;

    default:
        type = inputEvent::Type::INVALID;
        return;
    }
}

//------------------------------------------------------------------------------
inputEvent::MouseEvent InputEvent::initMouseEvent(MOUSE_EVENT_RECORD inEvent) {
    inputEvent::MouseEvent ret;

    // Get mouse position
    ret.mousePosition = Position{ inEvent.dwMousePosition.X,
            inEvent.dwMousePosition.Y };

    // Set event flag
    // The INPUT_RECORD indicates a forward scroll if high word of 
    // dwButtonState is positive. This means the most significant bit of
    // dwButtonState must be 0 to indicate a forward scroll. The hex value
    // 0x7FFFFFFF represents the 32-bit decimal value where all bits are set
    // to 1 except the most significant bit, which is set to 0. This is the
    // maximum value that dwButtonState can be while indicating a forward
    // scroll. 
    switch (inEvent.dwEventFlags) {
    case DOUBLE_CLICK:
        ret.eventFlag = inputEvent::Mouse::DOUBLE_CLICKED;
        break;

    case MOUSE_HWHEELED:
        if (inEvent.dwButtonState <= 0x7FFFFFFF) {
            ret.eventFlag = inputEvent::Mouse::WHEELED_RIGHT;
        }
        else {
            ret.eventFlag = inputEvent::Mouse::WHEELED_LEFT;
        }
        break;

    case MOUSE_MOVED:
        ret.eventFlag = inputEvent::Mouse::MOVED;
        break;

    case MOUSE_WHEELED:
        if (inEvent.dwButtonState <= 0x7FFFFFFF) {
            ret.eventFlag = inputEvent::Mouse::WHEELED_FORWARD;
        }
        else {
            ret.eventFlag = inputEvent::Mouse::WHEELED_BACKWARD;
        }
        break;

    default:
        ret.eventFlag = inputEvent::Mouse::CLICKED;
        break;
    }

    // Check left click
    if (inEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
        ret.leftClick = true;
    }
    else {
        ret.leftClick = false;
    }

    // Check right click
    if (inEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
        ret.rightClick = true;
    }
    else {
        ret.rightClick = false;
    }

    return ret;
}

//------------------------------------------------------------------------------
inputEvent::KeyEvent InputEvent::initKeyEvent(KEY_EVENT_RECORD inEvent) {
    inputEvent::KeyEvent ret;

    // Set event flag
    ret.eventFlag = inputEvent::Key::NONE;
    if (inEvent.dwControlKeyState & CAPSLOCK_ON) {
        ret.eventFlag = ret.eventFlag | inputEvent::Key::CAPSLOCK;
    }
    if (inEvent.dwControlKeyState & LEFT_ALT_PRESSED) {
        ret.eventFlag = ret.eventFlag | inputEvent::Key::ALT;
    }
    if (inEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
        ret.eventFlag = ret.eventFlag | inputEvent::Key::CTRL;
    }
    if (inEvent.dwControlKeyState & SHIFT_PRESSED) {
        ret.eventFlag = ret.eventFlag | inputEvent::Key::SHIFT;
    }

    // Set if keyed down
    ret.keyedDown = inEvent.bKeyDown;

    // Set repeat count
    ret.repeatCount = inEvent.wRepeatCount;

    // Set character
    ret.character = inEvent.uChar.AsciiChar;

    return ret;
}

//------------------------------------------------------------------------------
inputEvent::ResizeEvent InputEvent::initResizeEvent(WINDOW_BUFFER_SIZE_RECORD
    inEvent) {
    return inputEvent::ResizeEvent{ Position{ inEvent.dwSize.X,
            inEvent.dwSize.Y } };
}

}