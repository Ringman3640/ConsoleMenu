//------------------------------------------------------------------------------
// inputhookchain.h
// Interface for the InputHookChain class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An InputHookChain is a collection of input hooks that are called
//     in series given an InputEvent struct. Hooks are added into the chain in
//     list order. InputEvents are first processed by the most recently-added
//     hook and then passed down the list of hooks. Added hooks can be removed
//     from the chain given a handle to that hook.
//
// Class Functionality:
//     - Add an input hook and obtain a handle to that added hook. 
//     - Remove an input hook from the chain given it's handle.
//     - Start the hook chain given an InputEvent.
// 
// Dependencies: InputEvent struct.
//------------------------------------------------------------------------------

#pragma once

#include <functional>
#include "ConsoleEditor/inputevent.h"

namespace conu {

//------------------------------------------------------------------------------
// HookHandle class
// Defines a handle to a specific input hook handle within an InputHookChain
//     object.
// Used to remove an inserted hook from an InputHookChain.
class HookHandle {
    friend class InputHookChain;

public:
    //--------------------------------------------------------------------------
    // Default constructor
    HookHandle();

    //--------------------------------------------------------------------------
    // Parameter constructor
    HookHandle(std::function<void(InputEvent&)>* inHandle);

private:
    // Pointer to handle function
    std::function<void(InputEvent&)>* hookPtr;

};

//------------------------------------------------------------------------------
class InputHookChain {
public:

    //--------------------------------------------------------------------------
    // Default constructor
    InputHookChain();

    //--------------------------------------------------------------------------
    // Destructor
    ~InputHookChain();

    //--------------------------------------------------------------------------
    // Add an input hook to the start of the hook chain.
    // Returns a handle to the inserted hook to use for removal.
    HookHandle addInputHook(std::function<void(conu::InputEvent&)> hook);

    //--------------------------------------------------------------------------
    // Remove an inserted hook from the hook chain given the hook's handle.
    // Returns false if the HookHandle was not found. Otherwise returns true.
    bool removeInputHook(HookHandle& handle);

    //--------------------------------------------------------------------------
    // Execute the hook chain given an InputEvent struct.
    // The chain execution will terminate if the InputEvent type is assigned to
    //     INVALID at any point in the chain.
    void startHookChain(InputEvent& input) const;

    //--------------------------------------------------------------------------
    // Clear all hooks from the hook chain.
    void clear();

private:
    // Node struct
    // Helper structure for the hook chain linked list
    struct Node {
        Node* prev;
        Node* next;
        std::function<void(InputEvent&)> hook;

        Node();

    };

    // Linked list implementation for the node chain
    Node* head;
    Node* tail;
    int count;

};

}