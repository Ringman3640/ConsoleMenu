//------------------------------------------------------------------------------
// menurefreshaction.cpp
// Implementation for the MenuRefreshAction class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuReplyAction responsible for handling FEFRESH Replies.
//     When executed, calls for the target menu to print its contents.
// 
// Dependencies: MenuReplyAction class.
//------------------------------------------------------------------------------

#include "Menu/menureplyaction/menurefreshaction.h"

namespace conu {

//------------------------------------------------------------------------------
void MenuRefreshAction::execute(Menu& targetMenu) {
    targetMenu.print();
}

//------------------------------------------------------------------------------
MenuReplyAction* MenuRefreshAction::create() {
    return new MenuRefreshAction();
}

}