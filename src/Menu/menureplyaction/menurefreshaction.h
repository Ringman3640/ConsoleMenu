//------------------------------------------------------------------------------
// menurefreshaction.h
// Interface for the MenuRefreshAction class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuReplyAction responsible for handling FEFRESH Replies.
//     When executed, calls for the target menu to print its contents.
// 
// Dependencies: MenuReplyAction class.
//------------------------------------------------------------------------------

#pragma once

#include "Menu/MenuReplyAction/menureplyaction.h"

namespace conu {

//------------------------------------------------------------------------------
class MenuRefreshAction : public MenuReplyAction {
public:
    //--------------------------------------------------------------------------
    // Execute the intended refresh action
    virtual void execute(Menu& targetMenu) override;

    //--------------------------------------------------------------------------
    // Create a new instance of a MenuRefreshAction object
    // Allocates memory that must be deleted by the user
    virtual MenuReplyAction* create() override;

};

}