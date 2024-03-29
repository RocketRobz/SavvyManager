#include "characterChange.hpp"
#include "emblemChange.hpp"
#include "gameSelect.hpp"
#include "musicChange.hpp"
#include "stockManage.hpp"
#include "mewtubeChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

extern int ss1Logo;
extern int ss2Screenshot;
extern int ss2Logo;
extern int ss3Logo;
extern int ss4Logo;
extern int ss1LogoXpos;
extern int ss2LogoXpos;
extern int ss4LogoXpos;
extern bool ss2SaveFound;
extern bool ss3SaveFound;
extern bool ss4SaveFound;

WhatToDo::WhatToDo() {
	initialize();
}

void WhatToDo::initialize() {
	if ((highlightedGame == 0)
	|| (highlightedGame < 2 && whatToChange_cursorPosition == 2)) {
		whatToChange_cursorPosition = 0;
	}

	cursorChange();
}

void WhatToDo::Draw(void) const {
	extern bool doubleSpeed;
	if (doubleSpeed) {
		C3D_FrameRate(iFps);
		doubleSpeed = false;
	}

	GFX::loadGameSelSheets();

	Gui::ScreenDraw(Top);

	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(247, 206, 164, 255));
		bool light = true;
		int x = 0;
		int width = 29;
		for (int i = 0; i < 14; i++) {
			if (light) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(247, 231, 206, 255));
			}
			light = !light;
			x += width;
			width = light ? 29 : 28;
		}
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}

	switch(highlightedGame) {
		case 0:
		default:
			GFX::DrawGameSelSprite(ss1Logo, 40+ss1LogoXpos, 56);
			break;
		case 1:
			GFX::DrawGameSelSprite(ss2Logo, 40+ss2LogoXpos, 56);
			break;
		case 2:
			GFX::DrawGameSelSprite(ss3Logo, 40, 56);
			break;
		case 3:
			GFX::DrawGameSelSprite(ss4Logo, 40+ss4LogoXpos, 56);
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(247, 214, 181, 255));
		bool dark = true;
		int x = 0;
		int width = 17;
		for (int i = 0; i < 14; i++) {
			if (dark) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(247, 206, 164, 255));
			}
			dark = !dark;
			x += width;
			width = dark ? 29 : 28;
		}
		Gui::Draw_Rect(0, 0, 320, 33, C2D_Color32(247, 235, 206, 255));
		Gui::Draw_Rect(0, 33, 320, 5, C2D_Color32(214, 158, 58, 255));
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}

	Gui::DrawString(8, 8, 0.50, BLACK, "What do you want to change?");
	int iconXpos = (highlightedGame == 3 ? 32 : 64);
	int textXpos = (highlightedGame == 3 ? -96 : -64);
	const float textSize = 0.50;
	GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
	GFX::DrawSprite(sprites_icon_profile_idx, iconXpos, 80);
	Gui::DrawStringCentered(textXpos, 140, textSize, RED, "Characters");
	iconXpos += 64;
	textXpos += 64;
	if (highlightedGame > 1) {
		// Show stock option for Fashion Forward and Styling Star
		GFX::DrawSprite(sprites_icon_stock_idx, iconXpos, 84);
		Gui::DrawStringCentered(textXpos, 140, textSize, RED, "Stock");
	} else if (highlightedGame == 1) {
		// Show music pack option for Trendsetters
		GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
		GFX::DrawSprite(sprites_icon_music_idx, iconXpos, 80);
		Gui::DrawStringCentered(textXpos, 140, textSize, RED, "Music");
	}
	iconXpos += 64;
	textXpos += 64;
	if (highlightedGame > 1) {
		// Show emblem option for Fashion Forward and Styling Star
		GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
		GFX::DrawSprite(sprites_icon_emblem_idx, iconXpos, 80);
		Gui::DrawStringCentered(textXpos, 140, textSize, RED, "Emblem");
	}
	iconXpos += 64;
	textXpos += 64;
	if (highlightedGame == 3) {
		// Show Mewtube option for Styling Star
		GFX::DrawSprite(sprites_icon_mewtube_idx, iconXpos, 84);
		Gui::DrawStringCentered(textXpos, 140, textSize, RED, "Mewtube");
	}
	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);
	/*GFX::DrawSprite(sprites_button_shadow_idx, 251, 199);
	GFX::DrawSprite(sprites_button_blue_idx, 251, 195);*/
	GFX::drawCursor(cursorX, cursorY);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}

void WhatToDo::cursorChange() {
	if (highlightedGame == 3) {
		switch (whatToChange_cursorPosition) {
			case 0:
				cursorX = 48;
				cursorY = 104;
				break;
			case 1:
				cursorX = 116;
				cursorY = 104;
				break;
			case 2:
				cursorX = 180;
				cursorY = 104;
				break;
			case 3:
				cursorX = 244;
				cursorY = 104;
				break;
		}
	} else {
		switch (whatToChange_cursorPosition) {
			case 0:
			default:
				cursorX = 80;
				cursorY = 104;
				break;
			case 1:
				cursorX = 148;
				cursorY = 104;
				break;
			case 2:
				cursorX = 212;
				cursorY = 104;
				break;
		}
	}
}

void WhatToDo::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			showMessage = false;
		}
	} else {
		if (highlightedGame > 0 && showCursor) {
			if (hDown & KEY_LEFT) {
				sndHighlight();
				whatToChange_cursorPosition--;
				if (highlightedGame == 3) {
					if (whatToChange_cursorPosition < 0) whatToChange_cursorPosition = 3;
				} else if (highlightedGame > 1) {
					if (whatToChange_cursorPosition < 0) whatToChange_cursorPosition = 2;
				} else {
					if (whatToChange_cursorPosition < 0) whatToChange_cursorPosition = 1;
				}
				cursorChange();
			} else if (hDown & KEY_RIGHT) {
				sndHighlight();
				whatToChange_cursorPosition++;
				if (highlightedGame == 3) {
					if (whatToChange_cursorPosition > 3) whatToChange_cursorPosition = 0;
				} else if (highlightedGame > 1) {
					if (whatToChange_cursorPosition > 2) whatToChange_cursorPosition = 0;
				} else {
					if (whatToChange_cursorPosition > 1) whatToChange_cursorPosition = 0;
				}
				cursorChange();
			}
		}

		if (hDown & KEY_A) {
			runSelection = true;
		}

		if ((hDown & KEY_TOUCH) && touch.py >= 91 && touch.py <= 136) {
			if (highlightedGame == 3 && touch.px >= 39 && touch.px <= 279) {
				if (touch.px < 88) {
					whatToChange_cursorPosition = 0;
					cursorChange();
					runSelection = true;
				}
				if ((touch.px > 102) && (touch.px < 153)) {
					whatToChange_cursorPosition = 1;
					cursorChange();
					runSelection = true;
				}
				if ((touch.px > 166) && (touch.px < 217)) {
					whatToChange_cursorPosition = 2;
					cursorChange();
					runSelection = true;
				}
				if (touch.px > 230) {
					whatToChange_cursorPosition = 3;
					cursorChange();
					runSelection = true;
				}
			} else if (touch.px >= 71 && touch.px <= 248) {
				if (touch.px < 120) {
					whatToChange_cursorPosition = 0;
					cursorChange();
					runSelection = true;
				}
				if ((touch.px > 134) && (touch.px < 185) && (highlightedGame > 0)) {
					whatToChange_cursorPosition = 1;
					cursorChange();
					runSelection = true;
				}
				if ((touch.px > 198) && highlightedGame > 1) {
					whatToChange_cursorPosition = 2;
					cursorChange();
					runSelection = true;
				}
			}
		}
		
		if (runSelection) {
			sndSelect();
			switch (whatToChange_cursorPosition) {
				case 0:
					if ((highlightedGame==1 && ss2SaveFound)
					|| (highlightedGame==2 && ss3SaveFound)
					|| (highlightedGame==3 && ss4SaveFound))
					{
						Gui::setScreen(std::make_unique<CharacterChange>(), true);
					} else {
						sndBack();
						messageNo = 1;
						showMessage = true;
					}
					break;
				case 1:
					highlightedGame>1 ? Gui::setScreen(std::make_unique<StockManage>(), true) : Gui::setScreen(std::make_unique<MusicChange>(), true);
					break;
				case 2:
					Gui::setScreen(std::make_unique<EmblemChange>(), true);
					break;
				case 3:
					Gui::setScreen(std::make_unique<MewtubeChange>(), true);
					break;
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<GameSelect>(), true);
		}
	}
}