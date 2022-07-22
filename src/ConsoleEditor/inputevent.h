//------------------------------------------------------------------------------
// inputevent.h
// Interface for the InputEvent struct
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

#include <type_traits>
#include <windows.h>

namespace conu {

//------------------------------------------------------------------------------
// Position structure
// Contains a coordinate position relative to the some origin point. For the 
// console window, the origin point (0, 0) is the top left of the screen.
// Equivalent to the Windows COORD structure.
struct Position {
    int col;
    int row;
};

//------------------------------------------------------------------------------
// Boundary structure
// Contains the top left and bottom right coordinate points of a boundary
// rectangle. A Boundary represents some perimeter that an object is confined to
// when printed to the console screen.
// Equivalent to the Windows RECT structure.
struct Boundary {
    int left;
    int top;
    int right;
    int bottom;
};

//------------------------------------------------------------------------------
// inputEvent namespace
// Contains structures and enumerators used by the InputEvent class.
namespace inputEvent {
//--------------------------------------------------------------------------
// InputType enumerators
// Enumerators for the InputEvent struct. Indicates the event type held
// within the input union.
enum class Type {
    MOUSE_INPUT,        // Indicates a mouse input in 'info'
    KEY_INPUT,          // Indicates a keyboard input in 'info'
    RESIZE_INPUT,       // Indicates a screen resize event in 'info'
    INVALID             // Indicates an invalid input fron the console
};

//--------------------------------------------------------------------------
// Mouse enumerators
// Enumerators for the MouseEvent struct.
enum class Mouse {
    CLICKED,            // The left or right mouse button was clicked
    DOUBLE_CLICKED,     // The left or right mouse button was double clicked
    WHEELED_FORWARD,    // The scroll wheel was rolled away from the user
    WHEELED_BACKWARD,   // The scroll wheel was rolled towards the user
    WHEELED_LEFT,       // The scroll wheel was rolled left
    WHEELED_RIGHT,      // The scroll wheel was rolled right
    MOVED               // The mouse was moved
};

//--------------------------------------------------------------------------
// MouseEvent struct
// Contains information about a mouse event from the console input buffer.
struct MouseEvent {
    Position mousePosition;
    Mouse eventFlag;
    bool leftClick;
    bool rightClick;
};

//--------------------------------------------------------------------------
// Key enumerators
// Enumerators for the KeyEvent struct.
enum class Key {
    NONE = 0,        // Initializing enum, ignore
    CAPSLOCK = 1 << 0,   // Indicates that CAPSLOCK is on
    CTRL = 1 << 1,   // Indicates that left CTRL is held
    ALT = 1 << 2,   // Indicates that left ALT is held
    SHIFT = 1 << 3,   // Indicates that SHIFT is held
};

//--------------------------------------------------------------------------
// Key operator | overload
// Returns the logical OR combination of the left and right Key flags.
Key operator | (Key left, Key right);

//--------------------------------------------------------------------------
// Key operator & overload
// Returns the logical AND result of the left and right Key flags.
int operator & (Key left, Key right);

//--------------------------------------------------------------------------
// KeyEvent struct
// Contains information about a key event from the console input buffer.
struct KeyEvent {
    Key eventFlag;
    bool keyedDown;
    int repeatCount;
    char character;
};

//--------------------------------------------------------------------------
// ResizeEvent struct
// Contains information about a resize event from the console input buffer.
struct ResizeEvent {
    Position size;
};
}

//------------------------------------------------------------------------------
struct InputEvent {
public:
    //--------------------------------------------------------------------------
    // Struct constructors
    InputEvent(inputEvent::Type type);
    InputEvent(INPUT_RECORD inRecord);

    //--------------------------------------------------------------------------
    // InputEvent struct type that is initialized (listed below)
    inputEvent::Type type;

    //--------------------------------------------------------------------------
    // Union of InputEvent struct info
    union {
        inputEvent::MouseEvent mouse;
        inputEvent::KeyEvent key;
        inputEvent::ResizeEvent resize;
    } info;

private:
    //--------------------------------------------------------------------------
    // Helper initializer functions
    inputEvent::MouseEvent initMouseEvent(MOUSE_EVENT_RECORD inEvent);
    inputEvent::KeyEvent initKeyEvent(KEY_EVENT_RECORD inEvent);
    inputEvent::ResizeEvent initResizeEvent(WINDOW_BUFFER_SIZE_RECORD inEvent);

};

}
