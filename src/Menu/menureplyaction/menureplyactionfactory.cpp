//------------------------------------------------------------------------------
// menureplyactionfactory.cpp
// Implementation for the MenuReplyActionFactory class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: MenuReplyActionFactory is a class to create MenuReplyAction
//     objects given a Reply key. All recieved MenuReplyActions from this
//     factory must be deleted by the caller to free allocated memory.
// 
// Dependencies: Flag enums and MenuReplyAction class and derived classes.
//------------------------------------------------------------------------------

#include "Menu/menureplyaction/menureplyactionfactory.h"

namespace conu {

//------------------------------------------------------------------------------
// Static instance initialization
MenuReplyActionFactory MenuReplyActionFactory::instance;

//------------------------------------------------------------------------------
MenuReplyActionFactory::MenuReplyActionFactory() {
    // Add addiitonal MenuReplyActions here
    lookupTable.insert({ Reply::REFRESH, new MenuRefreshAction });
    lookupTable.insert({ Reply::EXIT, new MenuExitAction });
}

//------------------------------------------------------------------------------
MenuReplyActionFactory::~MenuReplyActionFactory() {
    for (auto& it : lookupTable) {
        delete it.second;
    }
}

//------------------------------------------------------------------------------
MenuReplyActionFactory& MenuReplyActionFactory::getInstance() {
    return instance;
}

//------------------------------------------------------------------------------
MenuReplyAction* MenuReplyActionFactory::getAction(Reply key) {
    auto it = lookupTable.find(key);
    if (it == lookupTable.end()) {
        return nullptr;
    }

    return it->second->create();
}

}