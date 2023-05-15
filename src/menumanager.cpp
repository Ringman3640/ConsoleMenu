//------------------------------------------------------------------------------
// menumanager.cpp
// Implementation for the MenuManager class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: The MenuManager object is responsible for automatically handling 
//     auto printing on a per-Menu basis. Menus are inserted into the
//     MenuManager in FILO behavior, where the most recently inserted Menu is
//     the active Menu that is being managed for auto print. 
// 
// Dependencies: Menu class.
//------------------------------------------------------------------------------

#include "Menu/menumanager.h"
#include "Menu/menu.h"

namespace conu {

//------------------------------------------------------------------------------
// Static member initialization
MenuManager MenuManager::instance;
ConsoleEditor& MenuManager::console = ConsoleEditor::getInstance();
int MenuManager::defaultFrameRate = 30;
int MenuManager::realtimeFrameRate = INVALID_FRAME_RATE;

//------------------------------------------------------------------------------
MenuManager::MenuManager() :
    restoreConsoleOnEmpty{ false },
    menuStack{ },
    frameInterval{ std::chrono::milliseconds(0) },
    threadTargetState{ ManagerState::INACTIVE },
    threadCurrentState{ ManagerState::INACTIVE },
    managerLock{ },
    managerCV{ } {

}

//------------------------------------------------------------------------------
MenuManager::~MenuManager() {
    stopFrameRateManager();
}

//------------------------------------------------------------------------------
MenuManager& MenuManager::getInstance() {
    return instance;
}

//------------------------------------------------------------------------------
void MenuManager::pushMenu(Menu& inMenu) {
    std::lock_guard<std::mutex> lock(stackLock);
    menuStack.push(&inMenu);
    update();
}

//------------------------------------------------------------------------------
void MenuManager::popMenu() {
    std::lock_guard<std::mutex> lock(stackLock);
    menuStack.pop();
    update();
}

//------------------------------------------------------------------------------
const Menu* MenuManager::peekMenu() {
    std::lock_guard<std::mutex> lock(stackLock);
    if (menuStack.empty()) {
        return nullptr;
    }
    return menuStack.top();
}

//------------------------------------------------------------------------------
void MenuManager::update() {
    if (menuStack.empty()) {
        if (restoreConsoleOnEmpty) {
            
            restoreConsoleOnEmpty = false;
        }
        stopFrameRateManager();
        return;
    }
    if (threadCurrentState == ManagerState::INACTIVE) {
        startFrameRateManager(ManagerState::PAUSED);
    }

    // Check if need to initialize ConsoleEditor
    if (!console.initialized()) {
        console.initialize();
        restoreConsoleOnEmpty = true;
    }

    MenuOptions currOptions = menuStack.top()->getOptions();
    pauseFrameRateManager();
    if (!currOptions.useAutoPrint) {
        return;
    }

    // Set frame interval
    static const int MILLI_IN_SECOND = 1000;
    frameInterval = std::chrono::milliseconds(MILLI_IN_SECOND);
    if (currOptions.frameRate < 0) {
        frameInterval /= defaultFrameRate;
    }
    else if (currOptions.frameRate < MINIMUM_FRAME_RATE) {
        frameInterval /= MINIMUM_FRAME_RATE;
    }
    else {
        frameInterval /= currOptions.frameRate;
    }

    resumeFrameRateManager();
}

//------------------------------------------------------------------------------
void MenuManager::setDefaultFrameRate(int frameRate) {
    if (frameRate < MINIMUM_FRAME_RATE) {
        frameRate = MINIMUM_FRAME_RATE;
    }
    defaultFrameRate = frameRate;
}

//------------------------------------------------------------------------------
int MenuManager::getDefaultFrameRate() {
    return defaultFrameRate;
}

//------------------------------------------------------------------------------
int& MenuManager::getLiveFrameRate() {
    return realtimeFrameRate;
}

//------------------------------------------------------------------------------
void MenuManager::refreshMenu() {
    std::lock_guard<std::mutex> lock(stackLock);
    if (menuStack.empty()) {
        return;
    }

    menuStack.top()->print();
}


//------------------------------------------------------------------------------
void MenuManager::frameRateManager() {
    threadCurrentState = ManagerState::ACTIVE;
    std::chrono::milliseconds prevPrintTime = getTimeMS();
    std::chrono::milliseconds prevFrameTime = getTimeMS();
    std::chrono::milliseconds currTime;
    int frameCount = 0;

    // prevPrintTime is used to mark the time point of the previous print()
    // prevFrameTime is used to mark the time point when frameCount is uploaded
    //     to realtimeFrameRate

    realtimeFrameRate = -1;
    while (true) {
        // Check for state changes
        // State change check inverted to reduce checks per loop iteration
        if (threadTargetState != ManagerState::ACTIVE) {
            // INACTIVE state
            if (threadTargetState == ManagerState::INACTIVE) {
                threadCurrentState = ManagerState::INACTIVE;
                realtimeFrameRate = -1;
                return;
            }
            if (threadTargetState == ManagerState::PAUSED) {
                threadCurrentState = ManagerState::PAUSED;

                std::unique_lock<std::mutex> lock(managerLock);
                managerCV.wait(lock);

                threadCurrentState = ManagerState::ACTIVE;
            }
        }

        // Check if need to draw frame
        currTime = getTimeMS();
        if (currTime - prevPrintTime >= frameInterval && stackLock.try_lock()) {
            menuStack.top()->print();
            prevPrintTime = currTime;
            ++frameCount;
            stackLock.unlock();
        }
        
        // Check if need to update realtime frame rate
        static const auto SECOND = std::chrono::seconds(1);
        if (currTime - prevFrameTime >= SECOND) {
            realtimeFrameRate = frameCount;
            prevFrameTime = currTime;
            frameCount = 0;
        }

    }
}

//------------------------------------------------------------------------------
std::chrono::milliseconds MenuManager::getTimeMS() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
}

//------------------------------------------------------------------------------
void MenuManager::startFrameRateManager(ManagerState defaultState) {
    if (threadCurrentState != ManagerState::INACTIVE) {
        return;
    }

    threadTargetState = defaultState;
    frameRateManagerThread = std::thread(&MenuManager::frameRateManager, this);

    // Block until state confirmed
    while (threadCurrentState != threadTargetState) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

//------------------------------------------------------------------------------
void MenuManager::stopFrameRateManager() {
    if (threadCurrentState == ManagerState::INACTIVE) {
        return;
    }

    resumeFrameRateManager();
    threadTargetState = ManagerState::INACTIVE;
    frameRateManagerThread.join();
}

//------------------------------------------------------------------------------
void MenuManager::pauseFrameRateManager() {
    if (threadCurrentState != ManagerState::ACTIVE) {
        return;
    }

    threadTargetState = ManagerState::PAUSED;

    // Block until thread state change confirmed
    while (threadCurrentState != threadTargetState) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

//------------------------------------------------------------------------------
void MenuManager::resumeFrameRateManager() {
    if (threadCurrentState != ManagerState::PAUSED) {
        return;
    }

    threadTargetState = ManagerState::ACTIVE;

    std::unique_lock<std::mutex> lock(managerLock);
    lock.unlock();

    // Notify until thread state change confirmed
    while (threadCurrentState != threadTargetState) {
        lock.lock();
        managerCV.notify_one();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

}