//------------------------------------------------------------------------------
// menu.h
// Interface for the Menu class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: 
//
// Class Functionality:
//     - 
// 
// Dependencies: Box, ConsoleEditor, and MenuReplyActionFactory class.
//------------------------------------------------------------------------------

#pragma once

#include "ConsoleEditor/consoleeditor.h"
#include "Menu/menumanager.h"
#include "Box/box.h"
#include "Box/BoxContainer/vertcontainer.h"

namespace conu {

class MenuReplyActionFactory;

//------------------------------------------------------------------------------
// 
struct MenuOptions {
    bool useBuffering;
    bool useAutoPrint;
    int frameRate;

    MenuOptions() :
        useBuffering{ true },
        useAutoPrint{ true },
        frameRate{ DEFAULT_FRAME_RATE } {
    
    }
};

//------------------------------------------------------------------------------
// 
class Menu {
public:
    Menu();
    virtual ~Menu();

    ItemAccessor operator [] (int layer);

    // Enter the Menu operation
    // Blocks until the Menu exits
    Reply enter();

    // Mark for the Menu to exit operation
    void exit();

    // Print the Menu contents to the console screen
    // Uses draw or buffer depending on menu options
    void print();

    // Set the current options of the menu
    void setOptions(const MenuOptions& options);

    // Get the current options of the menu
    const MenuOptions& getOptions() const;

    // Get a reference to the main vertical box container of the menu
    VertContainer& getContainer();

    // Set the Reply that is returned by the menu upon exit from enter()
    void setExitReply(Reply exitReply);

private:
    static ConsoleEditor& console;
    static MenuManager& manager;
    static MenuReplyActionFactory& actionFactory;

    VertContainer container;
    bool exitMenu;
    Reply exitReply;

    MenuOptions options;

    void entryLoop();

};

}
