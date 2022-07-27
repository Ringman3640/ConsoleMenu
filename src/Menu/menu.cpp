//------------------------------------------------------------------------------
// menu.cpp
// Implementation for the Menu class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: 
//
// Class Functionality:
//     - 
// 
// Dependencies: Box class and ConsoleEditor class.
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
    manager.pushMenu(*this);
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
    exitMenu = false;
    print();

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