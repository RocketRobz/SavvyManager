#ifndef _SAVVY_MANAGER_CHARACTER_CHANGE_HPP
#define _SAVVY_MANAGER_CHARACTER_CHANGE_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class CharacterChange : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	CharacterChange();
private:
	void preview() const;
	void getList();
	void getMaxChars();
	const char* seasonName(void) const;
	const char* characterName(bool showPlayerName) const;
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
		0: Character list
		1: What to change
		2: Body change
		3: Outfit change (Currently bow placement change)
		4: Import character
	*/

	mutable int cursorX, cursorY;
	int import_highlightedGame = 0;
	int seasonNo = 0;

	char chrFilePath[256];
	char chrFilePath2[256];

	bool displayNothing = false;

	mutable int charFadeAlpha = 0;
	bool previewCharacter = false;
	bool previewCharacterFound = false;

	bool showMessage = false;
	int messageNo = 0;
	char chararacterImported[48];
	bool removeBags = false;
	int zoomIn = 0;

	int cheatKeys[10] = {0};
	int cheatKeyPosition = 0;

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