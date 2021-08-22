#ifndef _SAVVY_MANAGER_MEWTUBE_CHANGE_HPP
#define _SAVVY_MANAGER_MEWTUBE_CHANGE_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class MewtubeChange : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	MewtubeChange();
private:
	void drawMsg(void) const;

	int subScreenMode = 0;
	/*
		0: Video list
		1: Which character to change
		2: Select character
	*/

	mutable int cursorX, cursorY;
	int cursorPosition = 0;
	int cursorPositionOnScreen[3] = {0};

	int videoShownFirst = 0;
	const int numberofVideos = 18;

	bool modeInited = false;

	bool showMessage = false;
	int messageNo = 0;
};

#endif