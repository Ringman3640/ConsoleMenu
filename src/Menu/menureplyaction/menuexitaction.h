//------------------------------------------------------------------------------
// menuexitaction.h
// Interface for the MenuExitAction class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuReplyAction responsible for handling EXIT Replies. When
//     executed, calls for the target menu to exit operation.
// 
// Dependencies: MenuReplyAction class.
//------------------------------------------------------------------------------

#pragma once

#include "Menu/menureplyaction/menureplyaction.h"

namespace conu {

class MenuExitAction : public MenuReplyAction {
public:
    //--------------------------------------------------------------------------
    // Execute the intended exit action
    virtual void execute(Menu& targetMenu) override;

    //--------------------------------------------------------------------------
    // Create a new instance of a MenuExitAction object
    // Allocates memory that must be deleted by the user
    virtual MenuReplyAction* create() override;

};

}
