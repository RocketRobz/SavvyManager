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
static int rrTextFade = 255;

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
		// Top half gradient
		C2D_DrawRectangle(
	64, 42, 0, 280, 76,
	C2D_Color32(82, 0, 121, 255), C2D_Color32(82, 0, 121, 255),
	C2D_Color32(169, 0, 254, 255), C2D_Color32(169, 0, 254, 255));
		// Bottom half gradient
		C2D_DrawRectangle(
	64, 118, 0, 280, 86,
	C2D_Color32(169, 0, 254, 255), C2D_Color32(169, 0, 254, 255),
	C2D_Color32(71, 0, 104, 255), C2D_Color32(71, 0, 104, 255));

		GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos-7, robzYpos, C2D_Color32(255, 0, 0, 127), 0.5, 1);
		GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos+7, robzYpos, C2D_Color32(0, 0, 255, 127), 0.5, 1);
		Gui::Draw_Rect(400-50, 0, 50, 240, C2D_Color32(0, 0, 0, 255));
		GFX::DrawSpriteLinear(sprites_rr_shapeBG_idx, 50, 0, 0.5, 1);
		Gui::Draw_Rect(0, 238, 400, 2, C2D_Color32(0, 0, 0, 255));	// Hide line from other texture(s)
		GFX::DrawSpriteLinear(sprites_rr_robz_idx, robzXpos, robzYpos, 0.5, 1);
		if (robzXpos == 135 && robzYpos == 19) {
			GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 54, 74, 0.5, 1);
			if (rrTextFade > 0) GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 54, 74, C2D_Color32(255, 255, 255, rrTextFade), 0.5, 1);
			rrTextFade -= 16;
			if (rrTextFade < 0) rrTextFade = 0;
		} else {
			robzXpos -= 4;
			robzYpos -= 4;
			if (robzXpos < 135) robzXpos = 135;
			if (robzYpos < 19) robzYpos = 19;
		}
	}
	if (delay > iFps*6 && rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
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


void RocketRobz::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if ((hDown & KEY_START || hDown & KEY_TOUCH) && subMode == 2) {
		delay = iFps*10;
	}
}