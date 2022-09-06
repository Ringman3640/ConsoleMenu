//------------------------------------------------------------------------------
// menu.cpp
// Implementation for the Menu class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: Menu is a wrapper for the VertContainer class. Each Menu is
//     responsible for a single VertContaner object and manages all inputs
//     sent to the window screen and the corresponding REPLY returned by the
//     VertContainer interaction. Menu objects can be printed to the window
//     screen at regular intervals using the auto print system. 
// 
// Dependencies: Box, ConsoleEditor, MenuReplyActionFactory, and InputHookChain
//      class.
//------------------------------------------------------------------------------

#include "Menu/menu.h"
#include "menureplyaction/menureplyactionfactory.h"

namespace conu {

//------------------------------------------------------------------------------
// Static member initialization
ConsoleEditor& Menu::console = ConsoleEditor::getInstance();
MenuManager& Menu::manager = MenuManager::getInstance();
MenuReplyActionFactory& Menu::actionFactory
        = MenuReplyActionFactory::getInstance();

//------------------------------------------------------------------------------
MenuOptions::MenuOptions() :
    printOnEnter{ true },
    backgroundTrans{ false },
    useBuffering{ true },
    useAutoPrint{ true },
    frameRate{ DEFAULT_FRAME_RATE } {

}

//------------------------------------------------------------------------------
Menu::Menu() :
    container{ VertContainer(MAXIMUM, MAXIMUM) },
    exitMenu{ false },
    exitReply{ Reply::CONTINUE } {

}

//------------------------------------------------------------------------------
Menu::~Menu() {

}

//------------------------------------------------------------------------------
ItemAccessor Menu::operator [] (int layer) {
    return container[layer];
}

//------------------------------------------------------------------------------
Reply Menu::enter() {
    exitMenu = false;
    manager.pushMenu(*this);

    if (options.printOnEnter) {
        print();
    }
    entryLoop();

    manager.popMenu();
    return exitReply;
}

//------------------------------------------------------------------------------
void Menu::print() {
    std::lock_guard<std::mutex> lock(printLock);

    container.backgroundTransparent(options.backgroundTrans);
    if (options.useBuffering) {
        container.buffer(Position{ 0, 0 }, console.getWindowBoundary());
        console.printWriteBuffer();
        return;
    }

    container.draw(Position{ 0, 0 }, console.getWindowBoundary());
}

//------------------------------------------------------------------------------
void Menu::insert(const Box& inBox) {
    container.insert(inBox);
}

//------------------------------------------------------------------------------
void Menu::insert(int layer, const Box& inBox) {
    container.insert(layer, inBox);
}

//------------------------------------------------------------------------------
void Menu::insert(int layer, const Box& inBox, const Position& pos) {
    container.insert(layer, inBox, pos);
}

void Menu::remove(int layer) {
    container.remove(layer);
}

//------------------------------------------------------------------------------
Box* Menu::get(int layer) const {
    return container.get(layer);
}

//------------------------------------------------------------------------------
Box* Menu::getRecent() const {
    return container.getRecent();
}

void Menu::setAlignment(Align alignment) {
    container.setAlignment(alignment);
}

//------------------------------------------------------------------------------
void Menu::exit() {
    exitMenu = true;
}

//------------------------------------------------------------------------------
void Menu::setOptions(const MenuOptions& options) {
    this->options = options;
}

//------------------------------------------------------------------------------
const MenuOptions& Menu::getOptions() const {
    return options;
}

//------------------------------------------------------------------------------
VertContainer& Menu::getContainer() {
    return container;
}

//------------------------------------------------------------------------------
void Menu::setExitReply(Reply exitReply) {
    this->exitReply = exitReply;
}

//------------------------------------------------------------------------------
HookHandle Menu::addInputHook(std::function<void(InputEvent&)> hook) {
    return hookChain.addInputHook(hook);
}

//------------------------------------------------------------------------------
bool Menu::removeInputHook(HookHandle& handle) {
    return hookChain.removeInputHook(handle);
}

//------------------------------------------------------------------------------
void Menu::entryLoop() {
    while (!exitMenu) {
        conu::InputEvent input = console.getButtonInput();
        hookChain.startHookChain(input);
        if (input.type != inputEvent::Type::MOUSE_INPUT) {
            continue;
        }

        Reply response = container.interact(input.info.mouse);
        MenuReplyAction* action = actionFactory.getAction(response);
        if (action == nullptr) {
            continue;
        }

        action->execute(*this);
        delete action;
    }
}

}