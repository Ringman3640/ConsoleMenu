//------------------------------------------------------------------------------
// menumanager.h
// Interface for the MenuManager class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuManager object is responsible for automatically handling 
//     auto printing on a per-Menu basis. Menus are inserted into the
//     MenuManager in FILO behavior, where the most recently inserted Menu is
//     the active Menu that is being managed for auto print. 
//
// Class Functionality:
//     - Get the singleton instance of the MenuManager class.
//     - Push and pop Menu objects into the manager.
//     - Update the MenuManager to reflect changes to Menu options.
//     - Set/get the default frame rate.
//     - Get the live frame rate of the auto printed Menus.
// 
// Dependencies: Menu class.
//------------------------------------------------------------------------------

#pragma once

#include <stack>
#include <thread>
#include <mutex>
#include <chrono>

namespace conu {

// The minimum frame rate allowed for auto print. 
const int MINIMUM_FRAME_RATE = 1;

// Indicate to use the default frame rate when specifying a frame rate value.
const int DEFAULT_FRAME_RATE = -1;

// Indicates that the class's real-time frame rate is invalid.
const int INVALID_FRAME_RATE = -1;

// Class declaration to prevent circular dependency.
// Menu interface included in MenuManager implementation file. 
class Menu;

class MenuManager {
public:
    //--------------------------------------------------------------------------
    // Destructor
    ~MenuManager();

    //--------------------------------------------------------------------------
    // Get the singleton instance of the MenuManager class.
    static MenuManager& getInstance();

    //--------------------------------------------------------------------------
    // Add a Menu to the MenuManager.
    void pushMenu(Menu& inMenu);

    //--------------------------------------------------------------------------
    // Remove the topmost Menu from the MenuManager.
    void popMenu();

    //--------------------------------------------------------------------------
    // Update the MenuManager to reflect any changes made to a Menu's options.
    void update();

    //--------------------------------------------------------------------------
    // Set the default frame rate of all Menus that specify to use the default
    //     frame rate.
    static void setDefaultFrameRate(int frameRate);

    //--------------------------------------------------------------------------
    // Get the current default frame rate.
    static int getDefaultFrameRate();

    //--------------------------------------------------------------------------
    // Get a reference to the live, real-time frame rate of the current Menu
    //     being auto printed.
    // If there is no Menu currently being auto printed, the reference value
    //     will be a negative value.
    static const int& getLiveFrameRate();

private:
    // ManagerState enumerator to indicate the target/current state of the 
    //     frame rate manager.
    enum class ManagerState {
        INACTIVE,
        ACTIVE,
        PAUSED
    };

    // Singleton static instance
    static MenuManager instance;

    // Static member data
    static int defaultFrameRate;
    static int realtimeFrameRate;

    // Member data
    std::stack<Menu*> menuStack;
    std::chrono::milliseconds frameInterval;

    // Frame rate manager thread members
    std::thread frameRateManagerThread;
    ManagerState threadTargetState;
    ManagerState threadCurrentState;

    // Thread control
    std::mutex stackLock;
    std::mutex managerLock;
    std::condition_variable managerCV;

    //--------------------------------------------------------------------------
    // Private constructor
    MenuManager();

    //--------------------------------------------------------------------------
    // Frame rate manager thread function.
    void frameRateManager();

    //--------------------------------------------------------------------------
    // Get the current time since epoch in milliseconds.
    // Helper method for frameRateManager().
    std::chrono::milliseconds getTimeMS() const;

    //--------------------------------------------------------------------------
    // Start the frame rate manager thread in the indicated state.
    void startFrameRateManager(ManagerState defaultState);

    //--------------------------------------------------------------------------
    // Stop the frame rate manager thread.
    void stopFrameRateManager();

    //--------------------------------------------------------------------------
    // Pause the frame rate manager thead until resumed.
    void pauseFrameRateManager();

    //--------------------------------------------------------------------------
    // Resume the frame rate manager that is in the paused state.
    void resumeFrameRateManager();

};

}
