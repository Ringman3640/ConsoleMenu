//------------------------------------------------------------------------------
// menureplyaction.h
// Interface for the MenuReplyAction class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: MenuReplyActions are responsible for operating on a Menu object
//     in response to a returned Reply from the Menu's vertical box container
//     after passing it input. 
// 
// Dependencies: Menu class.
//------------------------------------------------------------------------------

#pragma once

#include "Menu/menu.h"

namespace conu {

//------------------------------------------------------------------------------
class MenuReplyAction {
public:
    //--------------------------------------------------------------------------
    // Constructor
    MenuReplyAction() { }

    //--------------------------------------------------------------------------
    // Virtual destructor
    virtual ~MenuReplyAction() { }

    //--------------------------------------------------------------------------
    // Execute the intended reply action
    virtual void execute(Menu& targetMenu) = 0;

    //--------------------------------------------------------------------------
    // Create a new instance of the MenuReplyAction
    // Allocates memory that must be deleted by the user
    virtual MenuReplyAction* create() = 0;

};

}
