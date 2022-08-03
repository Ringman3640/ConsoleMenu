//------------------------------------------------------------------------------
// menu.h
// Interface for the Menu class
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
//     - Add input hooks to modify/respond to user inputs before they are 
//       processed.
// 
// Dependencies: Box, ConsoleEditor, MenuReplyActionFactory, and InputHookChain
//      class.
//------------------------------------------------------------------------------

#pragma once

#include <functional>
#include "ConsoleEditor/consoleeditor.h"
#include "Menu/menumanager.h"
#include "Menu/inputhookchain.h"
#include "Box/box.h"
#include "Box/BoxContainer/vertcontainer.h"

namespace conu {

class MenuReplyActionFactory;

//------------------------------------------------------------------------------
// MenuOptions structure
// Struct that defines the options used by a Menu object during operation. The
//     default options for a Menu are as follows:
//     
//     printOnEnter = true
//     useBuffering = true
//     useAutoPrint = true
//     frameRate    = DEFAULT_FRAME_RATE
struct MenuOptions {
    bool printOnEnter;      // Print the contents of the Menu to the Window
                            //     screen upon entry of the Menu. If set to
                            //     false, the window screen will not be updated
                            //     until the Menu is called to print through
                            //     the auto print system or a REFRESH reply.

    bool useBuffering;      // Use buffering to print the contents of the Menu
                            //     to the window screen. Otherwise, use drawing.

    bool useAutoPrint;      // Use the auto print system of MenuManager to
                            //     automatically print the contents of the Menu
                            //     at a regular frame rate.
    
    int frameRate;          // The target frame rate of the Menu when using auto
                            //     print. Indicate DEFAULT_FRAME_RATE to use the
                            //     default frame rate of MenuManager.
                            // Value ignored if useAutoPrint is false.

    // Default constructor
    MenuOptions();

};

//------------------------------------------------------------------------------
class Menu {
public:

    //--------------------------------------------------------------------------
    // Default constructor
    Menu();

    //--------------------------------------------------------------------------
    // Virtual destructor
    virtual ~Menu();

    //--------------------------------------------------------------------------
    // Obtain an ItemAccessor for a specific layer within the Menu's 
    //     BoxContainer.
    ItemAccessor operator [] (int layer);

    //--------------------------------------------------------------------------
    // Enter the Menu operation.
    // Blocks until the Menu exits.
    Reply enter();

    //--------------------------------------------------------------------------
    // Mark for the Menu to exit operation.
    void exit();

    //--------------------------------------------------------------------------
    // Print the Menu contents to the console screen.
    // Uses draw or buffer depending on menu options.
    void print();

    //--------------------------------------------------------------------------
    // Set the current options of the menu.
    void setOptions(const MenuOptions& options);

    //--------------------------------------------------------------------------
    // Get the current options of the menu.
    const MenuOptions& getOptions() const;

    //--------------------------------------------------------------------------
    // Get a reference to the main vertical box container of the menu.
    VertContainer& getContainer();

    //--------------------------------------------------------------------------
    // Set the Reply that is returned by the menu upon exit from enter().
    void setExitReply(Reply exitReply);

    //--------------------------------------------------------------------------
    // Add an input hook to the Menu's InputEvent processing pipeline.
    // An input hook is a procedure that is called-back whenever an InputEvent
    //     is processed by the Menu by the user's input. The procedure function
    //     must return void and must recieve a reference to a single InputEvent.
    // In the InputEvent processing pipeline, the most recently added hook is
    //     first called. The modified/unmodified InputEvent after completion of
    //     the first hook is then passed onto the next, most recent hook and
    //     so on until all hooks are called, or if the InputEvent type becomes
    //     INVALID.
    // Returns a handle to the added input hook to be used to remove the hook.
    HookHandle addInputHook(std::function<void(conu::InputEvent&)> hook);

    //--------------------------------------------------------------------------
    // Remove an input hook from the Menu given a handle to the added hook.
    // Returns true if the hook was successfully removed. Returns false if the
    //     hook was not contained in the Menu.
    bool removeInputHook(HookHandle& handle);

private:
    // Static member data
    static ConsoleEditor& console;
    static MenuManager& manager;
    static MenuReplyActionFactory& actionFactory;

    // Member data
    InputHookChain hookChain;
    VertContainer container;
    bool exitMenu;
    Reply exitReply;
    MenuOptions options;

    //--------------------------------------------------------------------------
    // Primary operation loop of the Menu object.
    virtual void entryLoop();

};

}
