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
// Class Functionality:
//     - Access/modify the Menu's VertContainer.
//     - Set/get the Menu's options.
//     - Automatically print the contents of the Menu at regular intervals.
// 
// Dependencies: Box, ConsoleEditor, and MenuReplyActionFactory class.
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
    if (options.useBuffering) {
        container.buffer(Position{ 0, 0 }, console.getWindowBoundary());
        console.printWriteBuffer();
        return;
    }

    container.draw(Position{ 0, 0 }, console.getWindowBoundary());
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
void Menu::entryLoop() {
    while (!exitMenu) {
        conu::InputEvent input = console.getButtonInput();
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