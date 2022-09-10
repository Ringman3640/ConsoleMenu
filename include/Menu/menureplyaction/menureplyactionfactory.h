//------------------------------------------------------------------------------
// menureplyactionfactory.h
// Interface for the MenuReplyActionFactory class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: MenuReplyActionFactory is a class to create MenuReplyAction
//     objects given a Reply key. All recieved MenuReplyActions from this
//     factory must be deleted by the caller to free allocated memory.
// 
// Dependencies: Flag enums and MenuReplyAction class and derived classes.
//------------------------------------------------------------------------------

#pragma once

#include <unordered_map>
#include "Menu/menureplyaction/menureplyaction.h"
#include "Menu/menureplyaction/menurefreshaction.h"
#include "Menu/menureplyaction/menuexitaction.h"

namespace conu {

//------------------------------------------------------------------------------
class MenuReplyActionFactory {
public:
    //--------------------------------------------------------------------------
    // Virtual destructor
    virtual ~MenuReplyActionFactory();

    //--------------------------------------------------------------------------
    // Get the singleton instance of the class.
    static MenuReplyActionFactory& getInstance();

    //--------------------------------------------------------------------------
    // Get a corresponding MenyReplyAction given a Reply
    // Allocated new memory which must be deleted by the caller
    MenuReplyAction* getAction(Reply key);

private:
    // Singleton instance
    static MenuReplyActionFactory instance;

    // Table of MenuReplyAction objects to create from
    std::unordered_map<Reply, MenuReplyAction*> lookupTable;

    //--------------------------------------------------------------------------
    // Private constructor
    MenuReplyActionFactory();

};

}
