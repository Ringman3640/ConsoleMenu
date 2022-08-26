//------------------------------------------------------------------------------
// flag.h
// File for the enumerator flags
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: Flags are enumerator classes that indicate some state, response,
//     or action. flag.h includes two enumerator flags: Align and Reply.
// 
//     Align indicates how content should be printed to the console relative
//     to the content's container. 
//
//     Reply indicates what action should be taken by the caller of a function
//     or method once the function or method returns. 
// 
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>

namespace conu {

//------------------------------------------------------------------------------
// Align enumerator
// Indicates the alignment of conent within a container. Align consists of
// horizontal and vertical alignment.
enum class Align {
    // Horizontal alignment
    LEFT        = 1 << 0,   // Content alignment along left border
    CENTER      = 1 << 1,   // Content centered horizontally
    RIGHT       = 1 << 2,   // Content alignment along right border

    // Vertical alignment
    TOP         = 1 << 3,   // Content alignment along top border
    MIDDLE      = 1 << 4,   // Content centered vertically
    BOTTOM      = 1 << 5    // Content alignment along bottom border
};

inline Align operator | (Align left, Align right) {
    return static_cast<Align>(
            static_cast<std::underlying_type<Align>::type>(left)
            | static_cast<std::underlying_type<Align>::type>(right));
}

inline int operator & (Align left, Align right) {
    return static_cast<int>(
        static_cast<std::underlying_type<Align>::type>(left)
        & static_cast<std::underlying_type<Align>::type>(right));
}

//------------------------------------------------------------------------------
// Reply enumerator
// Indicates a condition or operational message from a returning execution 
// function. 
enum class Reply {
    IGNORED     = 1 << 0,   // Executed no corresponding action
    CONTINUE    = 1 << 1,   // Executed sucessfully, continue operation
    REFRESH     = 1 << 2,   // Refresh the entire console screen
    REPEAT      = 1 << 3,   // Request the execution to be repeated
    FAILED      = 1 << 4,   // Failed to perform the requested action
    EXIT        = 1 << 5    // Indicate the callee container to exit
};

}
