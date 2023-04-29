//------------------------------------------------------------------------------
// entrytextbox.cpp
// Implementation for the EntryTextBox class
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Description: An EntryTextBox is a type of TextBox that allows the user to
//     input a text value when the box is interacted. The EntryTextBox stops
//     reading the user input when the Enter button is clicked or if the user
//     clicks off the box.
// 
// Dependencies: TextBox and MenuManager class.
//------------------------------------------------------------------------------

#include "Box/ContentBox/TextBox/entrytextbox.h"

namespace conu {

//------------------------------------------------------------------------------
EntryTextBox::EntryTextBox(std::string text) :
	TextBox(text),
	displayText{ text },
	userInteracting{ false },
    menuRefreshing{ false }{

}

//------------------------------------------------------------------------------
EntryTextBox::EntryTextBox(int width, int height, std::string text) :
	TextBox(width, height, text),
	displayText{ text },
	userInteracting{ false },
    menuRefreshing{ false }{

}

//------------------------------------------------------------------------------
// Note: I needed to add a discrete copy constructor since the class has a
// mutex, which cannot be copied. The default copy constructor would throw an
// error for this. Idk if there is a better solution.
EntryTextBox::EntryTextBox(const EntryTextBox& copy) :
        TextBox(copy) {
    displayText = copy.displayText;
    userInput = copy.userInput;
    userInteracting = copy.userInteracting;
    menuRefreshing = copy.menuRefreshing;
    inputHandler = copy.inputHandler;
    cancelHandler = copy.cancelHandler;
    processHandler = copy.processHandler;
}

//------------------------------------------------------------------------------
Reply EntryTextBox::interact(inputEvent::MouseEvent action) {
    userInteracting = true;
    tryMenuRefresh();

    while (true) {
        conu::InputEvent input = console.getButtonInput();

        // Ignore resize and invalid inputs
        if (input.type == inputEvent::Type::RESIZE_INPUT
                || input.type == inputEvent::Type::INVALID) {
            continue;
        }

        // Cancel interaction if user clicks off box; otherwise ignore mouse
        if (input.type == inputEvent::Type::MOUSE_INPUT) {
            if (!posInBounds(input.info.mouse.mousePosition)) {
                if (cancelHandler) {
                    textLock.lock();
                    cancelHandler(userInput);
                    textLock.unlock();
                }
                break;
            }
            continue;
        }

        // Ignore double inputs from keyup
        if (!input.info.key.keyedDown) {
            continue;
        }

        // Process interaction if Enter is pressed
        const char ENTER_CHAR = (char)13;
        if (input.info.key.character == ENTER_CHAR) {
            if (processHandler) {
                textLock.lock();
                processHandler(userInput);
                textLock.unlock();
            }
            break;
        }

        // Remove character if Backspace is pressed
        const char BACKSPACE_CHAR = (char)127;
        if (input.info.key.character == BACKSPACE_CHAR) {
            if (!userInput.empty()) {
                textLock.lock();
                userInput.pop_back();
                textLock.unlock();

                tryMenuRefresh();
            }
            continue;
        }

        // Ignore all input outside of the text ascii range
        const char START_ASCII_RANGE = 32;
        const char END_ASCII_RANGE = 126;
        if (input.info.key.character < START_ASCII_RANGE
                || input.info.key.character > END_ASCII_RANGE) {
            continue;
        }

        if (!inputHandler || inputHandler(input.info.key.character)) {
            textLock.lock();
            userInput.push_back(input.info.key.character);
            textLock.unlock();

            tryMenuRefresh();
        }
    }

    userInteracting = false;
    tryMenuRefresh();
    return Reply::CONTINUE;
}


//------------------------------------------------------------------------------
Box* EntryTextBox::copyBox() const {
	return new EntryTextBox(*this);
}

//------------------------------------------------------------------------------
Box* EntryTextBox::createBox() const {
	return new EntryTextBox();
}

//------------------------------------------------------------------------------
std::string EntryTextBox::getClassName() const {
	return std::string("EntryTextBox");
}

//------------------------------------------------------------------------------
void EntryTextBox::setText(std::string text) {
	this->text = text;
	displayText = text;
}

//------------------------------------------------------------------------------
void EntryTextBox::setInputHandler(std::function<bool(char)> handler) {
    inputHandler = handler;
}

//------------------------------------------------------------------------------
void EntryTextBox::setCancelHandler(std::function<void(std::string&)> handler) {
    cancelHandler = handler;
}

//------------------------------------------------------------------------------
void EntryTextBox::setProcessHandler(
        std::function<void(std::string&)> handler) {
    processHandler = handler;
}

//------------------------------------------------------------------------------
std::string EntryTextBox::getInput() const {
    return userInput;
}

//------------------------------------------------------------------------------
void EntryTextBox::clearInput() {
    userInput.clear();
}

//------------------------------------------------------------------------------
void EntryTextBox::autoMenuRefreshing(bool autoRefreshing) {
    menuRefreshing = autoRefreshing;
}

//------------------------------------------------------------------------------
Reply EntryTextBox::printProtocol(Position pos, Boundary container,
		bool drawMode) {
    std::lock_guard<std::mutex> lock(textLock);

	if (!userInteracting) {
        if (displayText.empty()) {
            text = userInput;
        }
        else {
            text = displayText;
        }
		return TextBox::printProtocol(pos, container, drawMode);
	}

    printBase(pos, container, drawMode);

    // Get amount of printable lines and printable width
    int temp = actualHeight - (horizBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableLines = static_cast<unsigned>(temp);

    temp = actualWidth - (vertBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableWidth = static_cast<unsigned>(temp);

    if (printableLines == 0 || printableWidth == 0 || userInput.empty()) {
        drawn = true;
        return conu::Reply::CONTINUE;
    }

    Position printPos = absolutePos;
    printPos.col += vertBorderSize;
    printPos.row += horizBorderSize;
    
    // If there is only one printable line, shift the content of visible text if
    // extending past the size of the box. This helps with user input visibility
    if (printableLines == 1) {
        std::string lineText;
        if (userInput.length() > printableWidth) {
            lineText = userInput.substr(userInput.length() - printableWidth);
        }
        else {
            lineText = userInput;
        }
        printLine(printPos, lineText.c_str(), drawMode);

        drawn = true;
        return conu::Reply::CONTINUE;
    }

    text = userInput;
    splitText();
    applyHorizontalAlignment();

    // Determine spacing offset for vertical alignment
    int vertOffset = 0;
    if (lines.size() < printableLines) {
        if (alignment & Align::TOP) {
            vertOffset = 0;
        }
        else if (alignment & Align::MIDDLE) {
            vertOffset = (printableLines - lines.size()) / 2;
        }
        else if (alignment & Align::BOTTOM) {
            vertOffset = printableLines - lines.size();
        }
    }

    // Line offset is used to keep last line of the entry box visible while the
    // user is typing input. Helps with visibility
    int lineOffset = lines.size() - printableLines;;
    if (lineOffset < 0) {
        lineOffset = 0;
    }

    // Print text
    printPos.row += vertOffset;
    for (unsigned i = 0; i < printableLines && i < lines.size(); ++i) {
        printLine(printPos, lines[i + lineOffset].c_str(), drawMode);
        ++printPos.row;
    }

    drawn = true;
    return Reply::CONTINUE;
}

//------------------------------------------------------------------------------
void EntryTextBox::tryMenuRefresh() const {
    if (!menuRefreshing) {
        return;
    }

    MenuManager::getInstance().refreshMenu();
}

/*
{
    // Get the printable lines for text within the EntryTextBox region
    int temp = actualHeight - (horizBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableLines = static_cast<unsigned>(temp);

    // Cannot print any lines
    if (printableLines == 0) {
        return Reply::CONTINUE;
    }

    // If there are two or more printable lines, print the text normally
    if (printableLines > 1) {
        return TextBox::printProtocol(pos, container, drawMode);
    }

    // If there is only one printable line, print the line so that the very end
    // of the user's input is visible. This helps with input visibility.

    printBase(pos, container, drawMode);
    if (userInput.empty()) {
        return Reply::CONTINUE;
    }

    // Get the printable width for the single printable line
    temp = actualWidth - (vertBorderSize * 2);
    if (temp < 0) {
        temp = 0;
    }
    unsigned printableWidth = static_cast<unsigned>(temp);

    // Cannot print any text
    if (printableWidth == 0) {
        return Reply::CONTINUE;
    }

    std::string lineText;

    // userInput needs to be padded for alignment
    if (userInput.length() < printableWidth) {
        int padding = printableWidth - userInput.length();
        if (alignment & Align::CENTER) {
            lineText = std::string(padding / 2, ' ') + userInput;
        }
        else if (alignment & Align::RIGHT) {
            lineText = std::string(padding, ' ') + userInput;
        }
        // Default is Align::LEFT, which does not need padding

        lineText = userInput;
    }

    // userInput fits exactly in the printable width
    else if (userInput.length() == printableWidth) {
        lineText = userInput;
    }

    // userInput exceeds printable width, need to shift content to the left
    else {
        lineText = userInput.substr(userInput.length() - printableWidth);
    }

    Position linePos = absolutePos;
    linePos.col += vertBorderSize;
    linePos.row += horizBorderSize;
    printLine(linePos, lineText.c_str(), drawMode);

    drawn = true;
    return Reply::CONTINUE;
}
*/

}