#ifndef _SAVVY_MANAGER_EXITING_HPP
#define _SAVVY_MANAGER_EXITING_HPP

#include "common.hpp"

class Exiting : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) override;
};

#endif