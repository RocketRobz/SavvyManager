#include "gameSelect.hpp"
#include "rocketRobz.hpp"
#include "screenvars.h"

static int subMode = 0;
static int prevSubMode = -1;
extern int delay;
static int rr_fadeAlpha = 0;
static int rr_fadeType = true;

extern u8 consoleModel;

void RocketRobz::Draw(void) const {
	/*if (!this->musicPlayed) {
		extern void musLogos(void);
		musLogos();
		this->musicPlayed = true;
	}*/

	Gui::ScreenDraw(Top);

	if (subMode == 2) {
		if (cinemaWide) {
			GFX::DrawSprite(sprites_logo_rocketrobz_idx, 60, 36, 0.35f, 0.7f, GPU_LINEAR);
		} else {
			GFX::DrawSprite(sprites_logo_rocketrobz_idx, 0, 0, 0.5);
		}
		Gui::DrawString(8, (cinemaWide ? 182 : 218), 0.50, WHITE, this->yearText);
	} else {
		GFX::DrawSprite(sprites_logo_savvymanager_idx, 56, 58);
	}
	if (delay > iFps*6 && rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	Gui::ScreenDraw(Bottom);
	if (subMode == 2) {
		//GFX::DrawSprite(sprites_logo_SSanniversary_idx, 32, 24);
		GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
	} else if (subMode == 1) {
		Gui::Draw_Rect(0, 0, 320, 240, WHITE);
		GFX::DrawSprite(sprites_logo_nintendoSynSophia_idx, 0, 0);
		Gui::DrawStringCentered(0, 8, 0.50, WHITE, this->gamesByText);
		if (sysRegion == CFG_REGION_KOR) {
			Gui::DrawStringCentered(0, 218, 0.50, BLACK, this->gameYearTextKOR);
		} else {
			Gui::DrawStringCentered(0, 218, 0.50, BLACK, sysRegion==CFG_REGION_JPN ? this->gameYearText : this->gameYearText2);
		}
	} else if (consoleModel == 3) {
		if (delay < iFps*3) delay = iFps*3;
	} else {
		//Gui::DrawStringCentered(0, 48, 0.50, WHITE, this->presentedText);
		//Gui::DrawStringCentered(0, 88, 0.60, WHITE, "Cinema");
		//GFX::DrawSprite(sprites_logo_widescreen_idx, 66, 107);
		Gui::DrawStringCentered(0, 48, 0.75, WHITE, this->presentedText);
		GFX::DrawSprite(sprites_logo_horiHD_idx, 32, 85);
	}
	if (rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	
	int fadeFPS;
	switch (iFps) {
		default:
			fadeFPS = 6;
			break;
		case 30:
			fadeFPS = 12;
			break;
		case 24:
			fadeFPS = 14;
			break;
	}
	if (subMode == 2) {
		fadeFPS += fadeFPS;
	}
	
	if (rr_fadeType) {
		rr_fadeAlpha -= fadeFPS;
		if (rr_fadeAlpha < 0) rr_fadeAlpha = 0;
	} else {
		rr_fadeAlpha += fadeFPS;
		if (rr_fadeAlpha > 255) rr_fadeAlpha = 255;
	}

	/*if (delay > iFps*13) {
		rr_fadeType = false;
	} else*/ if (delay > iFps*6) {
		if (prevSubMode != 1) {
			rr_fadeType = false;
		}
		if (rr_fadeAlpha == 255) {
			subMode = 2;
			rr_fadeType = true;
			prevSubMode++;
		}
	} else if (delay > iFps*3) {
		if (prevSubMode != 0) {
			rr_fadeType = false;
		}
		if (rr_fadeAlpha == 255) {
			subMode = 1;
			rr_fadeType = true;
			prevSubMode++;
		}
	}
}


void RocketRobz::Logic(u32 hDown, u32 hHeld, touchPosition touch) { }