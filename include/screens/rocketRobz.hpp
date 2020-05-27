#ifndef _SAVVY_MANAGER_ROCKETROBZ_HPP
#define _SAVVY_MANAGER_ROCKETROBZ_HPP

#include "common.hpp"

class RocketRobz : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	int text_width = 0;
	const char* yeartext = "2019-2020 RocketRobz";
	//const char* yeartext2 = "Games 2008-2017 Nintendo & syn Sophia";
};

#endif