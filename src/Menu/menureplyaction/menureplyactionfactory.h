//------------------------------------------------------------------------------
// menureplyactionfactory.h
// Interface for the MenuReplyActionFactory class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: MenuReplyActionFactory is a class to create MenuReplyAction
//     objects given a Reply key. All recieved MenuReplyActions from this
//     factory must be deleted by the caller to free allocated memory.
// 
// Dependencies: Flan enums and MenuReplyAction class and derived classes.
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
    // Constructor
    MenuReplyActionFactory();

    //--------------------------------------------------------------------------
    // Virtual destructor
    virtual ~MenuReplyActionFactory();

    //--------------------------------------------------------------------------
    // Get a corresponding MenyReplyAction given a Reply
    // Allocated new memory which must be deleted by the caller
    MenuReplyAction* getAction(Reply key);

private:
    std::unordered_map<Reply, MenuReplyAction*> lookupTable;

};

}