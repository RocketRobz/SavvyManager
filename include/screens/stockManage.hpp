#ifndef _SAVVY_MANAGER_STOCK_MANAGE_HPP
#define _SAVVY_MANAGER_STOCK_MANAGE_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class StockManage : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) override;
	StockManage();
private:
	void drawMsg(void) const;

	mutable int cursorX, cursorY;
	int cursorPosition = 0;

	const int numberofOptions = 1;
	bool boutiqueToo = true;

	bool showMessage = false;
	int messageNo = 0;
};

#endif