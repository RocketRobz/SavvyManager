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

static int robzXpos = 135+220;
static int robzYpos = 19+220;
static int rrFlashFade = 255;
static int rrTextFade = 255;
static int rrTextGlowFade = 127;
static int rrBgYpos = 240;

static int floorLineYpos[36] = {
	0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 33, 36, 39, 42, 45, 50, 54, 58, 62, 66, 70
};
static int floorLineCurrentYpos[3] = {0, 12, 24};
static bool floorLineMoveOdd[3] = {false, false, false};

#define rrTopGradient C2D_Color32(64, 0, 96, 255)
#define rrBottomGradient C2D_Color32(169, 0, 254, 255)
#define wireframeColor C2D_Color32(0, 255, 0, 255)

void RocketRobz::Draw(void) const {
	/*if (!this->musicPlayed) {
		extern void musLogos(void);
		musLogos();
		this->musicPlayed = true;
	}*/

	Gui::ScreenDraw(Top);

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
		bool robzInPos = (robzXpos == 135 && robzYpos == 19);
		if (robzInPos) {
			if (rrBgYpos > 0) {
				Gui::Draw_Rect(0, 0, 400, rrBgYpos, rrTopGradient);
			}
		// Top half gradient
		C2D_DrawRectangle(
	0, rrBgYpos, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	rrBottomGradient, rrBottomGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+120, 0, 400, 120,
	rrBottomGradient, rrBottomGradient,
	rrTopGradient, rrTopGradient);

		// Top half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+240, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	rrBottomGradient, rrBottomGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+240+120, 0, 400, 120,
	rrBottomGradient, rrBottomGradient,
	rrTopGradient, rrTopGradient);

		} else {
			// Ceiling
			C2D_DrawLine(0, 64, wireframeColor, 400, 64, wireframeColor, 1, 0);
			C2D_DrawLine(200, 0, wireframeColor, 200, 64, wireframeColor, 1, 0);
			int xTop = 0;
			int xBot = 0;
			for (int i = 0; i < 4; i++) {
				xTop += 80;
				xBot += 48;
				C2D_DrawLine(200-xTop, 0, wireframeColor, 200-xBot, 64, wireframeColor, 1, 0);
				C2D_DrawLine(200+xTop, 0, wireframeColor, 200+xBot, 64, wireframeColor, 1, 0);
			}
			for (int i = 0; i < 3; i++) {
				C2D_DrawLine(0, 64-floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 400, 64-floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 1, 0);
			}

			// Floor
			C2D_DrawLine(0, 240-64, wireframeColor, 400, 240-64, wireframeColor, 1, 0);
			C2D_DrawLine(200, 240-64, wireframeColor, 200, 240, wireframeColor, 1, 0);
			xTop = 0;
			xBot = 0;
			for (int i = 0; i < 4; i++) {
				xTop += 48;
				xBot += 80;
				C2D_DrawLine(200-xTop, 240-64, wireframeColor, 200-xBot, 240, wireframeColor, 1, 0);
				C2D_DrawLine(200+xTop, 240-64, wireframeColor, 200+xBot, 240, wireframeColor, 1, 0);
			}
			for (int i = 0; i < 3; i++) {
				C2D_DrawLine(0, (240-64)+floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 400, (240-64)+floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 1, 0);
			}

			for (int i = 0; i < 3; i++) {
				switch (iFps) {
					default:
						floorLineCurrentYpos[i]++;
						break;
					case 30:
						floorLineCurrentYpos[i]+=2;
						break;
					case 24:
						floorLineCurrentYpos[i] += 2+floorLineMoveOdd[i];
						floorLineMoveOdd[i] = !floorLineMoveOdd[i];
						break;
				}
				if (floorLineCurrentYpos[i] > 36) floorLineCurrentYpos[i] = floorLineMoveOdd[i];
			}

		// Top half gradient
		C2D_DrawRectangle(
	0, 0, 0, 400, 120,
	0, 0,
	rrTopGradient, rrTopGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, 120, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	0, 0);

		}
		if (robzInPos) {
			GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos-7, robzYpos, C2D_Color32(255, 0, 0, 127), 0.5, 1);
			GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos+7, robzYpos, C2D_Color32(0, 0, 255, 127), 0.5, 1);
			Gui::Draw_Rect(0, 0, 50, 240, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));
			Gui::Draw_Rect(400-50, 0, 50, 240, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));
			GFX::DrawSpriteLinearBlend(sprites_rr_shapeBG_idx, 50, 0, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255), 0.5, 1);
			Gui::Draw_Rect(0, 238, 400, 2, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));	// Hide line from other texture(s)
		}
		GFX::DrawSpriteLinear(sprites_rr_robz_idx, robzXpos, robzYpos, 0.5, 1);
		if (robzInPos) {
			if (rrTextFade != 255) GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 54, 74, 0.5, 1);
			if (rrTextFade > 0) {
				if (rrTextGlowFade > 0) {
					GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 39, 64, C2D_Color32(0, 255, 0, rrTextGlowFade), 0.55, 1.2);
				}
				GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 54, 74, C2D_Color32(0, 255, 0, rrTextFade), 0.5, 1);
			}
			switch (iFps) {
				default:
					rrFlashFade -= 16;
					rrBgYpos -= 2;
					break;
				case 30:
					rrFlashFade -= 32;
					rrBgYpos -= 4;
					break;
				case 24:
					rrFlashFade -= 40;
					rrBgYpos -= 6;
					break;
			}
			if (rrFlashFade < 0) {
				rrFlashFade = 0;
				switch (iFps) {
					default:
						rrTextFade -= 4;
						rrTextGlowFade -= 8;
						break;
					case 30:
						rrTextFade -= 8;
						rrTextGlowFade -= 16;
						break;
					case 24:
						rrTextFade -= 10;
						rrTextGlowFade -= 20;
						break;
				}
				if (rrTextFade < 0) rrTextFade = 0;
				if (rrTextGlowFade < 0) rrTextGlowFade = 0;
			}
			if (rrBgYpos < -240) rrBgYpos += 240;
		} else {
			switch (iFps) {
				default:
					robzXpos -= 2;
					robzYpos -= 2;
					break;
				case 30:
					robzXpos -= 4;
					robzYpos -= 4;
					break;
				case 24:
					robzXpos -= 6;
					robzYpos -= 6;
					break;
			}
			if (robzXpos < 135) robzXpos = 135;
			if (robzYpos < 19) robzYpos = 19;
		}
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
		Gui::DrawStringCentered(0, 48, 0.75, WHITE, this->presentedText);
		GFX::DrawSprite(sprites_logo_horiHD_idx, 32, 85);
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
		GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
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