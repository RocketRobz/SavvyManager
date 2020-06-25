#ifndef _SAVVY_MANAGER_PHOTO_STUDIO_HPP
#define _SAVVY_MANAGER_PHOTO_STUDIO_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class PhotoStudio : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	PhotoStudio();
private:
	void preview() const;
	void getList();
	void getMaxChars();
	const char* seasonName(void) const;
	const char* import_characterName(void) const;
	const char* import_SS2CharacterNames(int i) const;
	const char* import_characterNameDisplay(void) const;
	const char* ss1Title(void) const;
	const char* ss2Title(void) const;
	const char* ss3Title(void) const;
	const char* ss4Title(void) const;
	void drawMsg(void) const;
	void loadChrImage(bool Robz);
	void addEveryone(void);

	int subScreenMode = 0;
	/*
		0: What to change
		1: Studio BG list
		2: Character list
	*/

	mutable int cursorX, cursorY;
	int photo_highlightedGame = 0;
	int seasonNo = 0;

	char chrFilePath[256];
	char chrFilePath2[256];

	bool displayNothing = false;
	bool displayStudioBg = true;
	bool characterPicked = false;

	mutable int charFadeAlpha = 0;
	bool previewCharacter = false;
	bool previewCharacterFound = false;

	bool showMessage = false;
	int messageNo = 0;
	int zoomIn = 0;

	u16 totalCharacters = 0;
	u16 import_totalCharacters = 0;

	int characterList_cursorPosition = 0;
	int characterList_cursorPositionOnScreen = 0;

	int characterChangeMenu_cursorPosition = 0;
	int characterChangeMenu_cursorPositionOnScreen = 0;
	int characterChangeMenu_optionShownFirst = 0;
	int characterChangeMenuOps[4] = {0};
	int characterChangeMenuOptions = 2;

	int importCharacterList_cursorPosition = 0;
	int importCharacterList_cursorPositionOnScreen = 0;

	int characterShownFirst = 0;
	int import_characterShownFirst = 0;

	bool exportedCharListGotten[4] = {false};
};

#endif