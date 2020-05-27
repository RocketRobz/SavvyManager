#ifndef _SAVVY_MANAGER_GAMESELECT_HPP
#define _SAVVY_MANAGER_GAMESELECT_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class GameSelect : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	void drawCannotEditMsg(void) const;
	int messageNo = 0;
	bool showMessage = false;
	int cursorAlpha = 0;
};

#endif