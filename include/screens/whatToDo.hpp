#ifndef _SAVVY_MANAGER_WHAT_TO_DO_HPP
#define _SAVVY_MANAGER_WHAT_TO_DO_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class WhatToDo : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	WhatToDo();
private:
	void initialize();
	void cursorChange();
	bool runSelection = false;
	int whatToChange_cursorPosition = 0;
	bool showMessage = false;
	int messageNo = 0;
	int cursorX, cursorY;
};

#endif