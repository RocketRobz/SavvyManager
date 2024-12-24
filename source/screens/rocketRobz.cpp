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

static int robzXpos = 235+220;
static int robzYpos = 15+220;
static float robzXscale = 0.5;
static float robzYscale = 1;
static int rrFlashFade = 255;

void RocketRobz::Draw(void) const {
	/*if (!this->musicPlayed) {
		extern void musLogos(void);
		musLogos();
		this->musicPlayed = true;
	}*/

	Gui::ScreenDraw(Top);

	bool robzInPos = (robzXpos == 235 && robzYpos == 15);

	if (subMode == 2) {
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
		GFX::DrawSpriteLinear(sprites_rr_spaceBG_idx, 0, 0, 0.5, 1);

		if (robzInPos) {
			GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 115, 41, 0.5, 1);
			if (rrFlashFade > 0) {
				GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 115, 41, C2D_Color32(255, 255, 255, rrFlashFade), 0.5, 1);
			}
			switch (iFps) {
				default:
					rrFlashFade -= 16;
					break;
				case 30:
					rrFlashFade -= 32;
					break;
				case 24:
					rrFlashFade -= 40;
					break;
			}
			if (rrFlashFade < 0) {
				rrFlashFade = 0;
			}
		} else {
			switch (iFps) {
				default:
					robzXpos -= 2;
					robzYpos -= 2;
					robzXscale -= 0.0024;
					robzYscale -= 0.005;
					break;
				case 30:
					robzXpos -= 4;
					robzYpos -= 4;
					robzXscale -= 0.0048;
					robzYscale -= 0.010;
					break;
				case 24:
					robzXpos -= 6;
					robzYpos -= 6;
					robzXscale -= 0.0072;
					robzYscale -= 0.015;
					break;
			}
			if (robzXpos < 235) robzXpos = 235;
			if (robzYpos < 15) robzYpos = 15;
		}
		GFX::DrawSpriteLinear(sprites_rr_robz_idx, robzXpos, robzYpos, robzXscale, robzYscale);
	}
	if (delay > iFps*7 && rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	if (subMode == 2 && gfxIsWide()) {
		//GFX::DrawSprite(sprites_logo_SSanniversary_idx, 32, 24);
		//Gui::DrawStringCentered(0, 48, 0.50, WHITE, this->presentedText);
		//Gui::DrawStringCentered(0, 88, 0.60, WHITE, "Cinema");
		//GFX::DrawSprite(sprites_logo_widescreen_idx, 66, 107);
		Gui::DrawStringCentered(0, 72, 0.50, WHITE, this->presentedText);
		GFX::DrawSprite(sprites_logo_horizonHD_idx, 16, 95);
	} else if (subMode == 1) {
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
		if (robzInPos) {
			GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
			if (rrFlashFade > 0) {
				GFX::DrawSpriteBlend(sprites_logo_UniversalCore_idx, 0, 26, C2D_Color32(255, 255, 255, rrFlashFade));
			}
		}
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
	
	if (rr_fadeType) {
		rr_fadeAlpha -= fadeFPS;
		if (rr_fadeAlpha < 0) rr_fadeAlpha = 0;
	} else {
		rr_fadeAlpha += fadeFPS;
		if (rr_fadeAlpha > 255) rr_fadeAlpha = 255;
	}

	/*if (delay > iFps*13) {
		rr_fadeType = false;
	} else*/ if (delay > iFps*7) {
		if (prevSubMode != 1) {
			rr_fadeType = false;
		}
		if (rr_fadeAlpha == 255) {
			subMode = 2;
			rr_fadeType = true;
			prevSubMode++;
		}
	} else if (delay > iFps*4) {
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
	if (((hDown & KEY_A) || (hDown & KEY_START) || (hDown & KEY_TOUCH)) && subMode == 2) {
		delay = iFps*12;
	}
}