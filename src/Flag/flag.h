//------------------------------------------------------------------------------
// flag.h
// File for the enumerator flags
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: Flags are enumerator classes that indicate some state, response,
//     or action. flag.h includes two enumerator flags: Alignment and Reply.
// 
//     Alignment indicates how content should be printed to the console relative
//     to the content's container. 
//
//     Reply indicates what action should be taken by the caller of a function
//     or method once the function or method returns.
//
// File Functionality:
//     - Defines Alignment and Reply enumerator flags.
//     - Defines and implements logical AND and OR operator overlaods for 
//       Alignment and Reply flags. 
// 
//------------------------------------------------------------------------------

#pragma once

#include <type_traits>

//------------------------------------------------------------------------------
// Alignment enumerator
// Indicates the alignment of conent within a container. Alignment consists of
// horizontal and vertical alignment.
enum class Alignment {
    // Horizontal alignment
    LEFT        = 1 << 0,   // Content alignment along left border
    CENTER      = 1 << 1,   // Content centered horizontally
    RIGHT       = 1 << 2,   // Content alignment along right border

    // Vertical alignment
    TOP         = 1 << 3,   // Content alignment along top border
    MIDDLE      = 1 << 4,   // Content centered vertically
    BOTTOM      = 1 << 5    // Content alignment along bottom border
};

inline Alignment operator | (Alignment left, Alignment right) {
    return static_cast<Alignment>(
            static_cast<std::underlying_type<Alignment>::type>(left)
            | static_cast<std::underlying_type<Alignment>::type>(right));
}

inline int operator & (Alignment left, Alignment right) {
    return static_cast<int>(
        static_cast<std::underlying_type<Alignment>::type>(left)
        & static_cast<std::underlying_type<Alignment>::type>(right));
}

//------------------------------------------------------------------------------
// Reply enumerator
// Indicates a condition or operational message from a returning execution 
// function. 
enum class Reply {
    IGNORED         = 1 << 0,   // Executed no corresponding action
    CONTINUE        = 1 << 1,   // Executed sucessfully, continue operation
    REFRESH_SELF    = 1 << 2,   // Refresh the called execution
    REFRESH_SCREEN  = 1 << 3,   // Refresh the entire console screen
    REPEAT          = 1 << 4,   // Request the execution to be repeated
    FAILED          = 1 << 5,   // Failed to perform the requested action
    EXIT            = 1 << 6    // Indicate the callee container to exit
};

inline Reply operator | (Reply left, Reply right) {
    return static_cast<Reply>(
        static_cast<std::underlying_type<Reply>::type>(left)
        | static_cast<std::underlying_type<Reply>::type>(right));
}

inline int operator & (Reply left, Reply right) {
    return static_cast<int>(
        static_cast<std::underlying_type<Reply>::type>(left)
        & static_cast<std::underlying_type<Reply>::type>(right));
}