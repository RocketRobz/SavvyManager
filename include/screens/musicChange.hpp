#ifndef _SAVVY_MANAGER_MUSIC_CHANGE_HPP
#define _SAVVY_MANAGER_MUSIC_CHANGE_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class MusicChange : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) override;
	MusicChange();
private:
	void drawMsg(void) const;
	mutable int cursorX, cursorY;
	//int screenmode = 0;
	//int subScreenMode = 0;
	int cursorPosition = 0;
	int cursorPositionOnScreen = 0;

	int musicPackShownFirst = 0;

	bool modeInited = false;

	bool showMessage = false;
	int messageNo = 0;
};

#endif