//------------------------------------------------------------------------------
// asciipainter.cpp
// AsciiPainter program for drawing ascii art.
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Program Description: AsciiPainter is a console program for drawing ascii-
//     based pictures and graphics. 
// 
// Dependencies: ConsoleMenu Library
//     Library Link: https://github.com/Ringman3640/ConsoleMenu
//------------------------------------------------------------------------------

#define NOMINMAX
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include "consolemenu.h"

// Namespace for information regarding styling presets or effects.
namespace style {

conu::BorderFill thinFill{
        (char)219,
        (char)223,
        (char)219,
        (char)220 };

conu::BorderFill underlineFill{
        ' ',
        ' ',
        ' ',
        (char)220 };

// Effect for Button objects when clicked. If this effect function is called
// in a Button's action, the button will flash white while the mouse is
// hovered over the Button and holding left click. If left click is released
// over the Button boundaries, the function returns Reply::CONTINUE. Otherwise
// returns Reply::IGNORED.
conu::Reply clickEffect(conu::Button& button, 
        conu::inputEvent::MouseEvent input) {
    conu::ConsoleEditor& console = conu::ConsoleEditor::getInstance();
    conu::BorderSize oldSize = button.getBorderSize();
    conu::BorderSize flashSize{ conu::MAXIMUM, conu::MAXIMUM };

    while (input.leftClick) {
        if (button.posInBounds(input.mousePosition)) {
            button.setBorderSize(flashSize);
        }
        else {
            button.setBorderSize(oldSize);
        }

        conu::InputEvent temp = console.getRawInput();
        while (temp.type != conu::inputEvent::Type::MOUSE_INPUT) {
            temp = console.getRawInput();
        }
        input = temp.info.mouse;
    }

    button.setBorderSize(oldSize);
    if (button.posInBounds(input.mousePosition)) {
        return conu::Reply::CONTINUE;
    }
    return conu::Reply::IGNORED;
}

}

// Namespace for information regarding the screen dimensions for each
// interface, and contains the interfaces themselves.
namespace screen {

// ConsoleEditor instance
conu::ConsoleEditor& console = conu::ConsoleEditor::getInstance();

// Dimensions of the start menu window
int startWidth = 60;
int startHeight = 40;

// Dimensions of the paint menu window
int paintWidth = 100;
int paintHeight = 55;

// Base dimensions of the paint screen contents excluding the canvas.
// Used to make decisions on window sizing when resizing the canvas.
int paintBaseWidth = 80;
int paintBaseHeight = 8;

// Padding widths for menu screens.
int paddingHoriz = 2;
int paddingVert = 4;

// Interfaces
conu::Menu startMenu;
conu::Menu paintMenu;
conu::Menu resizeMenu;
conu::Menu exportMenu;

}

// Namespace for information regarding the drawing canvas in the paint
// interface.
namespace canvas {

// Pointer to the canvas object
conu::Graphic* canvasPtr = nullptr;
conu::HorizContainer* canvasBorderPtr = nullptr;

// Dimensions of the canvas object
int width = 80;
int height = 40;

// Min/max dimensions of the canvas (internal)
const int minWidth = 1;
const int minHeight = 1;
const int maxWidth = 230;
const int maxHeight = 48;

}

// Namespace for information regarding canvas resizing
namespace resize {

// Reference pointers to resize entry boxes
conu::EntryTextBox* entryWidthPtr = nullptr;
conu::EntryTextBox* entryHeightPtr = nullptr;

// Previous width and height values to revert changes
int prevWidth = canvas::width;
int prevHeight = canvas::height;

bool enterDigitHandler(char input) {
    if ((int)input < (int)'0') {
        return false;
    }
    if ((int)input > (int)'9') {
        return false;
    }
    return true;
}

void processWidthHandler(std::string& userInput) {
    int userVal;
    try {
        userVal = std::stoi(userInput);
    }
    catch (std::exception except) {
        userInput = std::to_string(canvas::width);
        return;
    }

    if (userVal < canvas::minWidth) {
        canvas::width = canvas::minWidth;
        userInput = std::to_string(canvas::minWidth);
        return;
    }
    if (userVal > canvas::maxWidth) {
        canvas::width = canvas::maxWidth;
        userInput = std::to_string(canvas::maxWidth);
        return;
    }
    canvas::width = userVal;
}
void processHeightHandler(std::string& userInput) {
    int userVal;
    try {
        userVal = std::stoi(userInput);
    }
    catch (std::exception except) {
        userInput = std::to_string(canvas::height);
        return;
    }

    if (userVal < canvas::minHeight) {
        canvas::height = canvas::maxHeight;
        userInput = std::to_string(canvas::minHeight);
        return;
    }
    if (userVal > canvas::maxHeight) {
        canvas::height = canvas::maxHeight;
        userInput = std::to_string(canvas::maxHeight);
        return;
    }
    canvas::height = userVal;
}

}

// Namespace for information regarding the brush selection region in the paint
// interface.
namespace selection {

char brush = (char)219;
int width = 80;

}

// Namespace for information regarding the utilities region in the paint
// interface.
namespace utilities {

int height = 3;
int buttonWidth = 16;

}

void initializeWindow();
void programStart();
void buildStartInterface();
void buildPaintInterface();
template <int charAscii>
void addBrushSelection(conu::HorizContainer& container);
void buildResizeInterface();
void buildExportInterface();
void buildLogic();
void resizeCanvas();
void resizePaintMenu();
void drawHandler(conu::InputEvent& input);
void keyHandler(conu::InputEvent& input);
void copyCanvas();
void exportCanvas();

int main() {
    initializeWindow();
    programStart();
}

// Initialization of the console window using ConsoleEditor
void initializeWindow() {
    screen::console.initialize();
    screen::console.allowWindowResizing(false);
    screen::console.allowMaximizeBox(false);
}

// Build and start the program interfaces
void programStart() {
    buildStartInterface();
    screen::startMenu.enter();

    buildPaintInterface();
    buildResizeInterface();
    buildExportInterface();
    buildLogic();

    resizePaintMenu();
    screen::paintMenu.enter();
}

// Build the start menu interface
void buildStartInterface() {
    // Make console clear on exit
    conu::MenuOptions options = screen::startMenu.getOptions();
    options.clearOnExit = true;
    screen::startMenu.setOptions(options);

    // Set start menu alignment
    screen::startMenu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

    //
    // Start logo
    //
    int logoWidth = 47;
    int logoHeight = 16;
    conu::Graphic startLogo(logoWidth, logoHeight);
    startLogo[0] = "       лл                                      ";
    startLogo[1] = "      ллл                                      ";
    startLogo[2] = "     лл л     ллллл    ллллл     лллл     л   л";
    startLogo[3] = "     л  л     л        лл       лл   л         ";
    startLogo[4] = "    лл  л     л         ллл    лл         л   л";
    startLogo[5] = "   ллллллл    ллл         лл   л          л   л";
    startLogo[6] = "  лл     л      ллл        л   л          л   л";
    startLogo[7] = " лл      л        л    лл  л   лл    л    л   л";
    startLogo[8] = "лл       л   лллллл     лллл    ллллл     л   л";
    startLogo[9] = "                                               ";
    startLogo[10] = "           лл                                  ";
    startLogo[11] = "          л  л              л                  ";
    startLogo[12] = "          лллл      л      ллл                 ";
    startLogo[13] = "          л    ллл  м  ллл  л  лппл лпл        ";
    startLogo[14] = "          л   л  л  л  л лл л  лммл л          ";
    startLogo[15] = "          л   ллллп л  л  л л  лммм л          ";

    screen::startMenu.insert(startLogo);
    screen::startMenu.insert(conu::Spacer(1, 1));

    //
    // Credit text
    //
    std::string creditString = "Created by Franz Alarcon";
    conu::TextBox creditText(logoWidth, 1, creditString);
    creditText.setAlignment(conu::Align::LEFT | conu::Align::TOP);

    screen::startMenu.insert(creditText);
    screen::startMenu.insert(conu::Spacer(1, 4));

    //
    // Start button
    //
    std::string startString = "Start";
    conu::Button startButton(startString.size() + 10, 3, startString);
    startButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    startButton.setBorderSize(1);
    startButton.setBorderFill(style::thinFill);
    startButton.setAction([](conu::Button& self, 
            conu::inputEvent::MouseEvent input)->conu::Reply {
                if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                    screen::startMenu.exit();
                }
                return conu::Reply::CONTINUE;
        });
    screen::startMenu.insert(startButton);

    // Set start menu screen dimensions
    int screenWidth = screen::startMenu.getContainer().getContentWidth()
            + screen::paddingVert * 2;
    int screenHeight = screen::startMenu.getContainer().getContentHeight()
            + screen::paddingHoriz * 2;
    screen::startMenu.setScreenDimensions(screenWidth, screenHeight);

}

// Build the paint menu interface
void buildPaintInterface() {
    // Change window size
    screen::console.setWindowDimensions(screen::paintWidth, screen::paintHeight);

    // Set paint menu alignment
    screen::paintMenu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

    //
    // Canvas region
    //
    conu::HorizContainer canvasBorder(canvas::width + 2, canvas::height + 2);
    canvasBorder.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    canvasBorder.setBorderSize(1);
    canvasBorder.setBorderFill(style::thinFill);
    screen::paintMenu.insert(canvasBorder);

    // Save border ptr reference
    canvas::canvasBorderPtr = dynamic_cast<conu::HorizContainer*>(
            screen::paintMenu.getRecent());

    // Canvas
    conu::Graphic canvas(canvas::width, canvas::height);
    canvas::canvasBorderPtr->insert(canvas);

    // Save canvas ptr reference
    canvas::canvasPtr = dynamic_cast<conu::Graphic*>(
            canvas::canvasBorderPtr->getRecent());

    //
    // Spacer 1
    //
    conu::Spacer spacer1(conu::MAXIMUM, 1);
    screen::paintMenu.insert(spacer1);

    //
    // Brush indicator region
    //
    conu::HorizContainer indicateRegion(selection::width, 1);
    indicateRegion.setAlignment(conu::Align::LEFT | conu::Align::TOP);

    // Brush indicator text
    std::string brushTextString = "Current Brush: ";
    conu::TextBox brushText(brushTextString.size(), 1, brushTextString);
    indicateRegion[1] = brushText;

    // Live brush indicator
    conu::LiveTextBox brushIndicator(1, 1);
    brushIndicator.setLiveVariable(selection::brush);
    indicateRegion[2] = brushIndicator;

    // Change brush text
    std::string changeBrushString = 
            "(!) Use keyboard to change brush";
    conu::TextBox changeBrushText(changeBrushString.size(), 1, 
            changeBrushString);
    indicateRegion.insert(3, changeBrushText, conu::Position{ 1000, 0 });

    screen::paintMenu.insert(indicateRegion);

    //
    // Spacer 2
    //
    conu::Spacer spacer2(conu::MAXIMUM, 1);
    screen::paintMenu.insert(spacer2);

    //
    // Brush selection region
    //
    conu::HorizContainer selectRegion(selection::width, 1);
    selectRegion.setAlignment(conu::Align::LEFT | conu::Align::MIDDLE);

    // Special Brush text
    std::string specialBrushString = "Special Brushes: ";
    conu::TextBox specialBrushText(specialBrushString.size(), 1, 
            specialBrushString);
    specialBrushText.setAlignment(conu::Align::LEFT | conu::Align::MIDDLE);
    selectRegion.insert(specialBrushText);

    // Add Special Brush Buttons
    addBrushSelection<219>(selectRegion);   // Block brush
    addBrushSelection<223>(selectRegion);   // Top half block brush
    addBrushSelection<220>(selectRegion);   // Bottom half block brush
    addBrushSelection<176>(selectRegion);   // Light checkered brush
    addBrushSelection<177>(selectRegion);   // Medium checkered brush
    addBrushSelection<178>(selectRegion);   // Strong checkered brush
    addBrushSelection<174>(selectRegion);   // Left dash brush
    addBrushSelection<175>(selectRegion);   // Right dash brush
    addBrushSelection<32>(selectRegion);    // Clear brush

    screen::paintMenu.insert(selectRegion);

    //
    // Spacer 3
    //
    conu::Spacer spacer3(conu::MAXIMUM, 1);
    screen::paintMenu.insert(spacer3);

    //
    // Utilities
    //
    conu::HorizContainer utilRegion(selection::width, utilities::height);
    utilRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    utilRegion.setDistribution(conu::BoxDistrib::SPACED_TO_EDGE);

    // Button template
    conu::Button utilButton(utilities::buttonWidth, utilities::height);
    utilButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    utilButton.setBorderSize(1);
    utilButton.setBorderFill(style::thinFill);

    // Clear button
    utilButton.setText("Clear");
    utilButton.setAction([](conu::Button& self, 
                conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                canvas::canvasPtr->clear();
            }
            return conu::Reply::CONTINUE;
        });
    utilRegion.insert(utilButton);
    utilRegion.insert(conu::Spacer(1, 1));

    // Copy button
    utilButton.setText("Copy");
    utilButton.setAction([](conu::Button& self,
                conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                copyCanvas();
            }
            return conu::Reply::CONTINUE;
        });
    utilRegion.insert(utilButton);
    utilRegion.insert(conu::Spacer(1, 1));

    // Export button
    utilButton.setText("Export");
    utilButton.setAction([](conu::Button& self,
                conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                exportCanvas();
                screen::exportMenu.enter();
            }
            return conu::Reply::CONTINUE;
        });
    utilRegion.insert(utilButton);
    utilRegion.insert(conu::Spacer(1, 1));

    // Resize button
    utilButton.setText("Resize");
    utilButton.setAction([](conu::Button& self,
                conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                resize::prevWidth = canvas::width;
                resize::prevHeight = canvas::height;
                screen::resizeMenu.enter();
            }
            return conu::Reply::CONTINUE;
        });
    utilRegion.insert(utilButton);

    screen::paintMenu.insert(utilRegion);

}

// Add a brush selection button to the given HorizContainer
// Helper function for buildPaintInterface()
template<int charAscii>
void addBrushSelection(conu::HorizContainer& container) {
    static conu::Button specialBrushButton;
    static char insertChar[4] = { '[', ' ', ']', '\0' };
    static bool initialized = false;
    if (!initialized) {
        specialBrushButton.setDimensions(4, 1);
        specialBrushButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
        initialized = true;
    }

    insertChar[1] = (char)charAscii;
    specialBrushButton.setText(insertChar);
    specialBrushButton.setAction([](conu::Button& self)->conu::Reply {
            selection::brush = (char)charAscii;
            return conu::Reply::CONTINUE;
        });
    container.insert(specialBrushButton);
}

void buildResizeInterface() {
    // Make menu transparent
    conu::MenuOptions options = screen::resizeMenu.getOptions();
    options.backgroundTrans = true;
    screen::resizeMenu.setOptions(options);

    // Set alignment to center/middle
    screen::resizeMenu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

    //
    // Resize control region
    //
    conu::VertContainer resizeRegion(44, 17);
    resizeRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    resizeRegion.setBorderSize(1);
    resizeRegion.setBorderFill(style::thinFill);

    //
    // Resize control text
    //
    std::string resizeCtrlStr = "Resize canvas size";
    conu::TextBox resizeCtrlText(resizeCtrlStr.length(), 1, resizeCtrlStr);
    resizeRegion.insert(resizeCtrlText);
    resizeRegion.insert(conu::Spacer(1, 1));

    //
    // Canvas width resize region
    //
    conu::HorizContainer resizeWidthRegion(1, 3);
    resizeWidthRegion.setAlignment(conu::Align::LEFT | conu::Align::MIDDLE);
    resizeWidthRegion.dynamicallySized(true);

    // Width text
    std::string resizeWidthStr = "Width   ";
    conu::TextBox resizeWidthText(resizeWidthStr.length(), 1, resizeWidthStr);
    resizeWidthRegion.insert(resizeWidthText);

    // Width entry box
    conu::EntryTextBox widthEntry(15, 3);
    widthEntry.setAlignment(conu::Align::CENTER | conu::Align::TOP);
    widthEntry.setBorderSize(1);
    widthEntry.setBorderFill(style::underlineFill);
    widthEntry.setInput(std::to_string(canvas::width));
    widthEntry.setInputHandler(resize::enterDigitHandler);
    widthEntry.setCancelHandler(resize::processWidthHandler);
    widthEntry.setProcessHandler(resize::processWidthHandler);
    resizeWidthRegion.insert(widthEntry);

    resizeRegion.insert(resizeWidthRegion);
    resizeRegion.insert(conu::Spacer(1, 1));

    //
    // Canvas height resize region
    //
    conu::HorizContainer resizeHeightRegion(1, 3);
    resizeHeightRegion.setAlignment(conu::Align::LEFT | conu::Align::MIDDLE);
    resizeHeightRegion.dynamicallySized(true);

    // Height text
    std::string resizeHeightStr = "Height  ";
    conu::TextBox resizeHeightText(resizeHeightStr.length(), 1, resizeHeightStr);
    resizeHeightRegion.insert(resizeHeightText);

    // height entry box
    conu::EntryTextBox heightEntry(15, 3);
    heightEntry.setAlignment(conu::Align::CENTER | conu::Align::TOP);
    heightEntry.setBorderSize(1);
    heightEntry.setBorderFill(style::underlineFill);
    heightEntry.setInput(std::to_string(canvas::height));
    heightEntry.setInputHandler(resize::enterDigitHandler);
    heightEntry.setCancelHandler(resize::processHeightHandler);
    heightEntry.setProcessHandler(resize::processHeightHandler);
    resizeHeightRegion.insert(heightEntry);

    resizeRegion.insert(resizeHeightRegion);
    resizeRegion.insert(conu::Spacer(1, 1));

    //
    // Button region
    //
    conu::HorizContainer buttonRegion(conu::MAXIMUM, 3);
    buttonRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

    // Button region template button
    conu::Button buttonRegionButton(16, 3);
    buttonRegionButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    buttonRegionButton.setBorderSize(1);
    buttonRegionButton.setBorderFill(style::thinFill);

    // Cancel button
    buttonRegionButton.setText("Cancel");
    buttonRegionButton.setAction([](conu::Button& self,
                conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) != conu::Reply::CONTINUE) {
                return conu::Reply::CONTINUE;
            }
            canvas::width = resize::prevWidth;
            canvas::height = resize::prevHeight;
            resize::entryWidthPtr->setInput(std::to_string(canvas::width));
            resize::entryHeightPtr->setInput(std::to_string(canvas::height));
            screen::resizeMenu.exit();
            return conu::Reply::CONTINUE;
        });
    buttonRegion.insert(buttonRegionButton);
    buttonRegion.insert(conu::Spacer(4, 1));

    // Confirm button
    buttonRegionButton.setText("Confirm");
    buttonRegionButton.setAction([](conu::Button& self,
        conu::inputEvent::MouseEvent input)->conu::Reply {
            if (style::clickEffect(self, input) != conu::Reply::CONTINUE) {
                return conu::Reply::CONTINUE;
            }
            screen::resizeMenu.exit();
            resizeCanvas();
            resizePaintMenu();
            return conu::Reply::CONTINUE;
        });
    buttonRegion.insert(buttonRegionButton);

    resizeRegion.insert(buttonRegion);
    screen::resizeMenu.insert(resizeRegion);

    // Get/save references to entryWidth and entryHeight
    // This process is insanely messy but this is unforunately the only way to 
    // get references to deeply-nested boxes within a contentbox. I may rework
    // the library to use smart pointers and for containers to hold pointer
    // references which should be much easier to access specific box elements.
    conu::VertContainer* resizeRegionPtr = 
            dynamic_cast<conu::VertContainer*>(screen::resizeMenu.getRecent());
    conu::HorizContainer* entryWidthRegionPtr = 
            dynamic_cast<conu::HorizContainer*>(resizeRegionPtr->get(3));
    resize::entryWidthPtr = 
            dynamic_cast<conu::EntryTextBox*>(entryWidthRegionPtr->get(2));

    conu::HorizContainer* entryHeightRegionPtr =
            dynamic_cast<conu::HorizContainer*>(resizeRegionPtr->get(5));
    resize::entryHeightPtr = 
            dynamic_cast<conu::EntryTextBox*>(entryHeightRegionPtr->get(2));


}

// Build the export notification menu interface
void buildExportInterface() {
    // Make menu transparent
    conu::MenuOptions options = screen::exportMenu.getOptions();
    options.backgroundTrans = true;
    screen::exportMenu.setOptions(options);

    // Set alignment to center/middle
    screen::exportMenu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

    //
    // Export notification region
    //
    conu::VertContainer notifRegion(40, 9);
    notifRegion.setDistribution(conu::BoxDistrib::EVEN_SPACED);
    notifRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    notifRegion.setBorderSize(1);
    notifRegion.setBorderFill(style::thinFill);

    // Notification text
    std::string notifString = "(!) Exported to \"output.txt\"";
    conu::TextBox notifText(notifString.size(), 1, notifString);
    notifRegion.insert(notifText);

    // Confirm button
    std::string confirmString = "Okay";
    conu::Button confirm(20, 3, confirmString);
    confirm.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
    confirm.setBorderSize(1);
    confirm.setBorderFill(style::thinFill);
    confirm.setAction([](conu::Button& self,
            conu::inputEvent::MouseEvent input)->conu::Reply {
                if (style::clickEffect(self, input) == conu::Reply::CONTINUE) {
                    screen::exportMenu.exit();
                }
                return conu::Reply::CONTINUE;
        });
    notifRegion.insert(confirm);

    screen::exportMenu.insert(notifRegion);
}

// Build any required logic hooks for the interfaces
void buildLogic() {
    // Add input handler
    screen::paintMenu.addInputHook(drawHandler);
    screen::paintMenu.addInputHook(keyHandler);
}

// Resize the paint canvas to the set values of canvas::width and canvas::height
void resizeCanvas() {
    if (canvas::width < canvas::minWidth) {
        canvas::width = canvas::minWidth;
    }
    else if (canvas::width > canvas::maxWidth) {
        canvas::width = canvas::maxWidth;
    }
    if (canvas::height < canvas::minHeight) {
        canvas::height = canvas::minHeight;
    }
    else if (canvas::height > canvas::maxHeight) {
        canvas::height = canvas::maxHeight;
    }

    canvas::canvasPtr->setDimensions(canvas::width, canvas::height);
    canvas::canvasBorderPtr->setDimensions(canvas::width + 2, 
            canvas::height + 2);
}

// Resize the paint menu dimenisons to fit the canvas dimensions
void resizePaintMenu() {
    int winWidth = std::max(screen::paintBaseWidth, canvas::width + 2);
    int winHeight = screen::paintBaseHeight + canvas::height + 2;
    winWidth += screen::paddingVert * 2;
    winHeight += screen::paddingHoriz * 2;

    screen::console.setWindowDimensions(winWidth, winHeight);
}

// Resize the window of a given menu. The window will be resized to fit the
// dimenions of the menu 
void resizeMenuWindow(conu::Menu& menu, int vertPadding, int horizPadding) {
    int winWidth = menu.getContainer().getWidth() + vertPadding * 2;
    int winHeight = menu.getContainer().getHeight() + horizPadding * 2;

    screen::console.setWindowDimensions(winWidth, winHeight);
}

// Input hook for the paint interface to manage drawing to the canvas
void drawHandler(conu::InputEvent& input) {
    static conu::ConsoleEditor& console = conu::ConsoleEditor::getInstance();

    if (canvas::canvasPtr == nullptr) {
        return;
    }

    if (input.type != conu::inputEvent::Type::MOUSE_INPUT
            || !canvas::canvasPtr->posInBounds(input.info.mouse.mousePosition)) {
        return;
    }

    while (input.type == conu::inputEvent::Type::MOUSE_INPUT 
            && input.info.mouse.leftClick) {

        conu::Position mousePos = input.info.mouse.mousePosition;
        //mousePos.col--;
        if (canvas::canvasPtr->posInBounds(mousePos)) {
            conu::Position offset = canvas::canvasPtr->getPosition();
            (*canvas::canvasPtr)[mousePos.row - offset.row]
                    [mousePos.col - offset.col] = selection::brush;
        }

        input = console.getRawInput();
    }
}

// Input hook for the paint interface to manage keyboard inputs
void keyHandler(conu::InputEvent& input) {

    if (input.type != conu::inputEvent::Type::KEY_INPUT) {
        return;
    }

    if (input.info.key.eventFlag == conu::inputEvent::Key::CTRL) {
        // Do something
        return;
    }

    if (input.info.key.keyedDown) {
        selection::brush = input.info.key.character;
    }

}

// Copy the contents of the drawing canvas to the Winows clipboard
void copyCanvas() {
    std::string clipData = canvas::canvasPtr->getString();
    HGLOBAL globalData = GlobalAlloc(GMEM_MOVEABLE, clipData.size() + 1);
    if (globalData != NULL) {
        memcpy(GlobalLock(globalData), clipData.c_str(), clipData.size() + 1);
    }
    else {
        return;
    }

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, globalData);
    CloseClipboard();
}

// Export the contents of the drawing canvas to a text file
void exportCanvas() {
    std::ofstream fout;

    fout.open("output.txt");
    if (fout.fail()) {
        fout.close();
        return;
    }

    fout << canvas::canvasPtr->getString() << std::endl;
    fout.close();
}