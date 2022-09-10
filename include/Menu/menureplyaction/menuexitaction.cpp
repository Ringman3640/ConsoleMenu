//------------------------------------------------------------------------------
// menuexitaction.cpp
// Implementation for the MenuExitAction class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuReplyAction responsible for handling EXIT Replies. When
//     executed, calls for the target menu to exit operation.
// 
// Dependencies: MenuReplyAction class.
//------------------------------------------------------------------------------

#include "Menu/menureplyaction/menuexitaction.h"

namespace conu {

//------------------------------------------------------------------------------
void MenuExitAction::execute(Menu& targetMenu) {
    targetMenu.exit();
}

//------------------------------------------------------------------------------
MenuReplyAction* MenuExitAction::create() {
    return new MenuExitAction();
}

}