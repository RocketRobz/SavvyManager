#include "gameSelect.hpp"
#include "rocketRobz.hpp"
#include "screenvars.h"

extern char verText[32];
extern u8 sysRegion;

static int subMode = 0;
static int prevSubMode = -1;
extern int delay;
static int rr_fadeAlpha = 0;
static int rr_fadeType = true;

void RocketRobz::Draw(void) const {
	/*if (!this->musicPlayed) {
		extern void musLogos(void);
		musLogos();
		this->musicPlayed = true;
	}*/

	Gui::ScreenDraw(Top);

	if (subMode == 1) {
		GFX::DrawSprite(sprites_logo_savvymanager_idx, 56, 58);
		if (gfxIsWide()) {
			// Replicate dot by dot image
			for (int i = 32; i < 400-32; i++) {
				Gui::Draw_Rect(i, 0, 0.5, 240, C2D_Color32(0, 0, 0, 127));
			}
		}
		Gui::DrawString(120, 218-(shiftBySubPixel ? 0.5f : 0), 0.50, WHITE, yearText);
		GFX::DrawSpriteLinear(sprites_text_rocketrobz_idx, 120+79, 220-(shiftBySubPixel ? 0.5f : 0), 0.5, 1);
		Gui::DrawString(8, 8-(shiftBySubPixel ? 0.5f : 0), 0.50, WHITE, verText);
	} else {
		GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 80, 0, 0.5, 1);
	}
	if (rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	if (subMode == 1) {
		Gui::Draw_Rect(0, 0, 320, 120, C2D_Color32(230, 0, 18, 255));
		Gui::Draw_Rect(0, 120, 320, 120, WHITE);
		GFX::DrawSprite(sprites_logo_nintendoSynSophia_idx, 0, 48);
		Gui::DrawStringCentered(0, 8, 0.50, WHITE, this->gamesByText);
		if (sysRegion == CFG_REGION_KOR) {
			Gui::DrawStringCentered(0, 218, 0.50, BLACK, this->gameYearTextKOR);
		} else {
			Gui::DrawStringCentered(0, 218, 0.50, BLACK, sysRegion==CFG_REGION_JPN ? this->gameYearText : this->gameYearText2);
		}
	} else if (subMode == 0) {
		GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
	}
	if (rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	
	int fadeFPS;
	switch (iFps) {
		default:
			fadeFPS = 12;
			break;
		case 30:
			fadeFPS = 24;
			break;
		case 24:
			fadeFPS = 32;
			break;
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
	} else*/ if (delay > iFps*2) {
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


void RocketRobz::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
	if ((hDown & KEY_A) || (hDown & KEY_START) || (hDown & KEY_TOUCH)) {
		if (subMode == 1 && delay < iFps*5) {
			delay = iFps*5;
		} else if (subMode == 0 && delay < iFps*2) {
			extern void stopMusLogos();
			stopMusLogos();

			delay = iFps*2;
		}
	}
}