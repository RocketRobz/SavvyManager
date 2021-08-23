#ifndef _SAVVY_MANAGER_EMBLEM_CHANGE_HPP
#define _SAVVY_MANAGER_EMBLEM_CHANGE_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class EmblemChange : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	EmblemChange();
private:
	void getMaxEmblems();
	int getPalNumber(u8 byte, bool secondPixel);
	u32 emblemPixel(int pixel, bool secondPixel);
	u32 emblemImage[64*64];
	void renderEmblem(void);
	void drawMsg(void) const;

	int subScreenMode = 0;
	/*
		0: Emblem list
		1: What to change
		2: Import emblem
	*/

	mutable int cursorX, cursorY;
	int importPage = 0;
	char embFilePath[256];
	bool displayNothing = false;

	int cursorPosition = 0;
	int emblemChangeMenu_cursorPosition = 0;
	int importEmblemList_cursorPosition = 0;
	int importEmblemList_cursorPositionOnScreen = 0;
	int totalEmblems = 0;
	int import_emblemShownFirst = 0;

	bool modeInited = false;
	bool showMessage = false;
	int messageNo = 0;
	char emblemImported[48];

	bool exportedEmblemListGotten = false;
};

#endif