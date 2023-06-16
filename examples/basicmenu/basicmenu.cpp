//------------------------------------------------------------------------------
// basicmenu.cpp
// BasicMenu program for demonstrating CONU.
// Author: Franz Alarcon
//------------------------------------------------------------------------------
// Program Description: BasicMenu is a demonstration program to showcase the
//     CONU library.
// 
// Dependencies: ConsoleMenu Library
//     Library Link: https://github.com/Ringman3640/ConsoleMenu
//------------------------------------------------------------------------------

#include <string>
#include <Windows.h>
#include <shellapi.h>
#include "consolemenu.h"

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

}

class Screen {
public:
	conu::Menu& getMenu() {
		return menu;
	}

protected:
	conu::ConsoleEditor& console = conu::ConsoleEditor::getInstance();
	conu::Menu menu;

};

class ProjectLinkScreen : public Screen {
public:
	ProjectLinkScreen() {

		// Configure Menu
		conu::MenuOptions options = menu.getOptions();
		options.backgroundTrans = true;
		menu.setOptions(options);
		menu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

		// 
		// Content region
		// 
		conu::VertContainer contentRegion;
		contentRegion.setBorderSize(1);
		contentRegion.setBorderFill(style::thinFill);
		contentRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

		// Notification banner text
		std::string notifBannerText = "(!) Open browser tab?";
		conu::TextBox notifBanner(notifBannerText.size(), 3, notifBannerText);
		contentRegion.insert(notifBanner);
		contentRegion.insert(conu::Spacer(1, 1));

		// Description text
		conu::TextBox description(40, 3, "The Console Menu GitHub project page "
				"will be opened in a new browser tab.");
		contentRegion.insert(description);
		contentRegion.insert(conu::Spacer(1, 1));

		// Button region
		conu::HorizContainer buttonRegion(40, 3);
		buttonRegion.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
		buttonRegion.setDistribution(conu::BoxDistrib::SPACED_TO_EDGE);

		// Cancel button
		conu::ExitButton cancelButton(16, 3, "Cancel");
		cancelButton.setBorderSize(1);
		cancelButton.setBorderFill(style::thinFill);
		cancelButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
		buttonRegion.insert(cancelButton);

		// Continue button
		conu::ExitButton continueButton(16, 3, "Continue");
		continueButton.setBorderSize(1);
		continueButton.setBorderFill(style::thinFill);
		continueButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
		continueButton.setAction([](conu::Button& self)->conu::Reply {
			ShellExecuteW(
					NULL,
					L"open",
					L"https://github.com/Ringman3640/ConsoleMenu",
					NULL,
					NULL,
					SW_SHOW);
			return conu::Reply::CONTINUE;
		});
		buttonRegion.insert(continueButton);
		contentRegion.insert(buttonRegion);

		// Set region dimensions with padding
		const int padding = 4;
		int regionWidth = contentRegion.getContentWidth() + padding * 2;
		int regionHeight = contentRegion.getContentHeight() + padding;
		contentRegion.setDimensions(regionWidth, regionHeight);

		menu.insert(contentRegion);
	}
};

class AboutScreen : public Screen {
public:
	AboutScreen() {
		// Configure Menu
		conu::MenuOptions options = menu.getOptions();
		options.printOnEnter = true;
		menu.setOptions(options);
		menu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

		// 
		// Header text box
		// 
		conu::TextBox header(5, 1, "About");
		menu.insert(header);
		menu.insert(conu::Spacer(1, 1));

		// 
		// About text area
		// 
		// The about text area Box is initialized with a height and width of 0.
		// The actual dimensions will be calculated in the applyTextDimenions()
		// method.
		conu::ScrollingTextBox aboutSection(0, 0);
		aboutSection.setHorizontalBorderSize(1);
		aboutSection.setBorderFill(conu::BorderFill{
				' ',
				(char)220,
				' ',
				(char)223 }
		);
		aboutSection.setAlignment(conu::Align::LEFT | conu::Align::TOP);
		aboutSection.setText(
			"ConsoleMenu (CONU) is a C++ GUI library for creating visual "
			"applications in the Windows default console. CONU is "
			"purpose-built for creating clickable menu systems within "
			"applications with support for real-time 2D interactive programs, "
			"such as basic games and drawing canvases.\n"
			"\n"
			"As indicated by the library's name, CONU was specifically "
			"designed for use designing menu interfaces. Menu systems within "
			"CONU support user inputs from the mouse and keyboard. This allows "
			"users to interact with programs using clickable on-screen "
			"buttons.\n"
			"\n"
			"This example program serves as a demonstration for what the CONU "
			"library can accomplish as a menu GUI. Users that read this text "
			"have navigated from the main menu screen and have selected the "
			"\"About\" button. Hello, users! \n"
			"\n"
			"The source code for this program can be found under examples/basic"
			"menu/basicmenu.cpp in the project GitHub page. This page can be "
			"accessed by clicking the \"Return\" button below to return to the "
			"main menu and then clicking the \"GitHub Page\" button. This will "
			"bring up a notification asking if the user would like open the "
			"page in a new browser tab.\n"
			"\n"
			"I created this library just as a personal project to create a "
			"GUI. I had always been interested in developing interactable "
			"programs, which I really could not do in standard C++ without an "
			"external GUI library. The implementation ideas came to me while I "
			"was sitting on the bus going to and from university, specifically "
			"the implementation details for how I could get a textbox to work "
			"(you are looking at a textbox right now!). I eventually decided "
			"that this would be a fun idea to create, so I got started and "
			"this is the result so far.\n"
			"\n"
			"This About page has gotten really rambly. I just needed a lot of "
			"text to showcase the scrolling feature in CONU for special "
			"textboxes. If you are reading this, bazinga."
		);
		menu.insert(aboutSection);
		aboutSectionPtr = menu.getRecent();
		menu.insert(conu::Spacer(1, 1));

		// 
		// Return button
		// 
		conu::ExitButton returnButton(14, 3, "Return");
		returnButton.setBorderSize(1);
		returnButton.setBorderFill(style::thinFill);
		returnButton.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);
		menu.insert(returnButton);
	}

	// Apply dimensions to the About page Box elements. This needs to be
	// performed outside of the constructor since the console window will not be
	// correctly sized at that time.
	// 
	// This is a messy solution resulting from the limitations of the current
	// CONU library. 
	void applyTextDimensions() {
		// Window dimension measurements
		const int winWidth = console.getWindowWidth();
		const int winHeight = console.getWindowHeight();
		const int padding = 4;
		int usableWidth = winWidth - padding * 2;
		int usableHeight = winHeight - padding;

		// About page format:
		// Page contents                             Box type         Box Height
		// ---------------------------------------------------------------------
		//                  About                 // TextBox          1
		//                                        // Spacer           1
		//  ____________________________________  //
		//                                        //
		//                                        //
		//                                        //
		//             About text area            // ScrollingTextBox (Fill)
		//                                        //
		//                                        //
		//  ____________________________________  //
		//                                        // Spacer           1
		//            ----------------            //
		//            |    Return    |            // ExitButton       3
		//            ----------------            //
		// ---------------------------------------------------------------------
		// Total absolute height: 6

		// Apply dimenions
		int aboutSectionHeight = usableHeight - 6;
		aboutSectionPtr->setDimensions(conu::Position{ usableWidth, 
				aboutSectionHeight });
	}

private:
	conu::Box* aboutSectionPtr = nullptr;

};

class HomeScreen : public Screen {
public:
	HomeScreen() {

		// Configure Menu
		conu::MenuOptions options = menu.getOptions();
		options.clearOnExit = true;
		menu.setOptions(options);
		menu.setAlignment(conu::Align::CENTER | conu::Align::MIDDLE);

		// 
		// Header logo graphic
		// 
		conu::Graphic headerLogo(47, 11);
		headerLogo[0] = "ÛßßßÛ  ÛßßßÛ  Û   Û  ÛßßßÛ  ÛßßßÛ  Û      ÛßßßÛ";
		headerLogo[1] = "Û      Û   Û  ÛÛ  Û  Û      Û   Û  Û      Û    ";
		headerLogo[2] = "Û      Û   Û  Û Û Û  ßßßßÛ  Û   Û  Û      Ûßß  ";
		headerLogo[3] = "Û      Û   Û  Û  ÛÛ      Û  Û   Û  Û      Û    ";
		headerLogo[4] = "ÛÜÜÜÛ  ÛÜÜÜÛ  Û   Û  ÛÜÜÜÛ  ÛÜÜÜÛ  ÛÜÜÜÛ  ÛÜÜÜÛ";
		headerLogo[5] = "                                               ";
		headerLogo[6] = "Û   Û  ÛßßßÛ  Û   Û  Û   Û                     ";
		headerLogo[7] = "ÛÛ ÛÛ  Û      ÛÛ  Û  Û   Û                     ";
		headerLogo[8] = "Û ß Û  Ûßß    Û Û Û  Û   Û                     ";
		headerLogo[9] = "Û   Û  Û      Û  ÛÛ  Û   Û                     ";
		headerLogo[10] = "Û   Û  ÛÜÜÜÛ  Û   Û  ÛÜÜÜÛ                     ";
		menu.insert(headerLogo);
		menu.insert(conu::Spacer(1, 1));

		int contentWidth = headerLogo.getWidth();

		// 
		// Attribution text
		// 
		menu.insert(conu::TextBox(contentWidth, 1, "By Franz Alarcon"));
		menu.insert(conu::Spacer(1, 2));

		// 
		// Project page button
		// 
		conu::MenuButton projectPage(contentWidth, 3, " GitHub Page");
		projectPage.setBorderSize(1);
		projectPage.setBorderFill(style::thinFill);
		projectPage.setEntryMenu(projectLink.getMenu());
		menu.insert(projectPage);
		menu.insert(conu::Spacer(1, 1));

		// 
		// About button
		// 
		conu::MenuButton aboutButton(contentWidth, 3, " About");
		aboutButton.setBorderSize(1);
		aboutButton.setBorderFill(style::thinFill);
		aboutButton.setEntryMenu(about.getMenu());
		aboutButton.setAction([&](conu::Button& self)->conu::Reply {
			about.applyTextDimensions();
			return conu::Reply::CONTINUE;
		});
		menu.insert(aboutButton);
		menu.insert(conu::Spacer(1, 1));

		// 
		// Exit button
		// 
		conu::ExitButton exitButton(contentWidth, 3, " Exit");
		exitButton.setBorderSize(1);
		exitButton.setBorderFill(style::thinFill);
		menu.insert(exitButton);

		// Set window dimensions with padding
		const int padding = 6;
		int menuWidth = menu.getContainer().getContentWidth() + padding * 2;
		int menuHeight = menu.getContainer().getContentHeight() + padding;
		menu.setScreenDimensions(menuWidth, menuHeight);
	}

	void enter() {
		menu.enter();
	}

private:
	ProjectLinkScreen projectLink;
	AboutScreen about;

};

int main() {
	conu::ConsoleEditor& console = conu::ConsoleEditor::getInstance();
	console.initialize();
	console.setCursorVisibility(false);
	console.allowWindowResizing(false);
	console.allowMaximizeBox(false);

	HomeScreen home;
	home.enter();

	console.restore();
}