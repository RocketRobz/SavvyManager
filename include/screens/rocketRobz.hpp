#ifndef _SAVVY_MANAGER_ROCKETROBZ_HPP
#define _SAVVY_MANAGER_ROCKETROBZ_HPP

#include "common.hpp"

class RocketRobz : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	//bool musicPlayed = false;
	const char* presentedText = "Presented in";
	const char* yearText = "2019-2020 RocketRobz";
	const char* gamesByText = "Original games by";
	const char* gameYearText = "2008-2017";
	const char* gameYearText2 = "2009-2017";
};

#endif