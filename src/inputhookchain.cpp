//------------------------------------------------------------------------------
// inputhookchain.cpp
// Implementation for the InputHookChain class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An InputHookChain is a collection of input hooks that are called
//     in series given an InputEvent struct. Hooks are added into the chain in
//     list order. InputEvents are first processed by the most recently-added
//     hook and then passed down the list of hooks. Added hooks can be removed
//     from the chain given a handle to that hook.
// 
// Dependencies: InputEvent struct.
//------------------------------------------------------------------------------

#include "Menu/inputhookchain.h"

namespace conu {

//------------------------------------------------------------------------------
HookHandle::HookHandle() :
    hookPtr{ nullptr } {

}

//------------------------------------------------------------------------------
HookHandle::HookHandle(std::function<void(InputEvent&)>* inHandle) :
    hookPtr{ inHandle } {
    
}

//------------------------------------------------------------------------------
InputHookChain::Node::Node() :
    prev{ nullptr },
    next{ nullptr },
    hook{ } {

}

//------------------------------------------------------------------------------
InputHookChain::InputHookChain() :
    head{ nullptr },
    tail{ nullptr },
    count{ 0 } {

}

//------------------------------------------------------------------------------
InputHookChain::~InputHookChain() {
    clear();
}

//------------------------------------------------------------------------------
HookHandle InputHookChain::addInputHook(std::function<void(InputEvent&)> hook) {
    Node* oldHead = head;
    head = new Node();
    head->hook = hook;
    head->next = oldHead;

    if (oldHead != nullptr) {
        oldHead->prev = head;
    }

    ++count;
    return HookHandle(&head->hook);
}

//------------------------------------------------------------------------------
bool InputHookChain::removeInputHook(HookHandle& handle) {
    // Check handle or empty chain
    if (handle.hookPtr == nullptr || count == 0) {
        return false;
    }

    // Search for hook in chain
    Node* traversal = head;
    while (true) {
        if (traversal == nullptr) {
            return false;
        }

        if (&traversal->hook == handle.hookPtr) {
            break;
        }

        traversal = traversal->next;
    }

    // Remove hook
    if (count == 1) {               // Removal node is head and tail
        head = nullptr;
        tail = nullptr;
    }
    else if (traversal == head) {   // Removal node is only head
        head = traversal->next;
        head->prev = nullptr;
    }
    else if (traversal == tail) {   // Removal node is only tail
        tail = traversal->prev;
        tail->next = nullptr;
    }
    else {                          // Removal node is an internal
        traversal->next->prev = traversal->prev;
        traversal->prev->next = traversal->next;
    }
    delete traversal;
    handle.hookPtr = nullptr;
    --count;
    return true;
}

//------------------------------------------------------------------------------
void InputHookChain::startHookChain(InputEvent& input) const {
    Node* traversal = head;
    while (traversal != nullptr && input.type != inputEvent::Type::INVALID) {
        traversal->hook(input);
        traversal = traversal->next;
    }
}

//------------------------------------------------------------------------------
void InputHookChain::clear() {
    Node* traversal = head;

    while (traversal != nullptr) {
        Node* nextNode = traversal->next;
        delete traversal;
        traversal = nextNode;
    }

    count = 0;
}

}