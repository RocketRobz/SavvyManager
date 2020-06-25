#ifndef _SAVVY_MANAGER_TITLESCREEN_HPP
#define _SAVVY_MANAGER_TITLESCREEN_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class titleScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	void drawCannotEditMsg(void) const;
	int messageNo = 0;
	bool showMessage = false;
	int cursorAlpha = 0;
	const char* yearText = "2019-2020 RocketRobz";
	const char* manageSaveText = "Manage Save Data";
	const char* photoStudioText = "Photo Studio";
	const char* settingsText = "Settings";
	const char* returnToHomeText = ": Return to HOME Menu";
	const char* exitText = "X: Exit";
};

#endif