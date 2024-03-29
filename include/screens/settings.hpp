#ifndef _SAVVY_MANAGER_SETTINGS_HPP
#define _SAVVY_MANAGER_SETTINGS_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class Settings : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) override;
private:
	const char* studioBgName(void) const;
	mutable int cursorX, cursorY;
	int numberOfSettings = 0;
	int cursorPosition = 0;
	int cursorPositionOnScreen = 0;
	int settingShownFirst = 0;
	//bool modeInited = false;
	//int screenmode = 0;
	//int subScreenMode = 0;
	
};

#endif