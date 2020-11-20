#include "characterChange.hpp"
#include "emblemChange.hpp"
#include "gameSelect.hpp"
#include "musicChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

extern int ss1Logo;
extern int ss2Screenshot;
extern int ss2Logo;
extern int ss1LogoXpos;
extern int ssLogoXpos;
extern int ss3Logo;
extern int ss4Logo;
extern bool ss2SaveFound;
extern bool ss3SaveFound;
extern bool ss4SaveFound;

WhatToDo::WhatToDo() {
	initialize();
}

void WhatToDo::initialize() {
	if ((highlightedGame == 0)
	|| (highlightedGame > 1 && whatToChange_cursorPosition == 1)
	|| (highlightedGame < 2 && whatToChange_cursorPosition == 2)) {
		whatToChange_cursorPosition = 0;
	}

	cursorChange();
}

void WhatToDo::Draw(void) const {
	GFX::loadGameSelSheets();

	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	switch(highlightedGame) {
		case 0:
		default:
			GFX::DrawGameSelSprite(ss1Logo, 40+ss1LogoXpos, 56);
			break;
		case 1:
			GFX::DrawGameSelSprite(ss2Logo, 40+ssLogoXpos, 56);
			break;
		case 2:
			GFX::DrawGameSelSprite(ss3Logo, 40, 56);
			break;
		case 3:
			GFX::DrawGameSelSprite(ss4Logo, 40+ssLogoXpos, 56);
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	Gui::DrawString(8, 8, 0.50, BLACK, "What do you want to change?");
	int iconXpos = 64;
	GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
	GFX::DrawSprite(sprites_icon_profile_idx, iconXpos, 80);
	Gui::DrawString(iconXpos-2, 140, 0.50, RED, "Characters");
	iconXpos += 64;
	if (highlightedGame == 1) {
		// Show music pack option for Trendsetters
		GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
		GFX::DrawSprite(sprites_icon_music_idx, iconXpos, 80);
		Gui::DrawString(iconXpos+14, 140, 0.50, RED, "Music");
	}
	iconXpos += 64;
	if (highlightedGame > 1) {
		// Show emblem option for Fashion Forward and Styling Star
		GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
		GFX::DrawSprite(sprites_icon_emblem_idx, iconXpos, 80);
		Gui::DrawString(iconXpos+8, 140, 0.50, RED, "Emblem");
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

void WhatToDo::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			showMessage = false;
		}
	} else {
		if (highlightedGame > 0 && showCursor) {
			if (hDown & KEY_LEFT) {
				sndHighlight();
				if (highlightedGame > 1) {
					if (whatToChange_cursorPosition == 2)	whatToChange_cursorPosition = 0;
					else if (whatToChange_cursorPosition == 0)	whatToChange_cursorPosition = 2;
					cursorChange();
				} else {
					whatToChange_cursorPosition--;
					if (whatToChange_cursorPosition < 0)	whatToChange_cursorPosition = 1;
					cursorChange();
				}
			} else if (hDown & KEY_RIGHT) {
				sndHighlight();
				if (highlightedGame > 1) {
					if (whatToChange_cursorPosition == 0) whatToChange_cursorPosition = 2;
					else if (whatToChange_cursorPosition == 2) whatToChange_cursorPosition = 0;
					cursorChange();
				} else {
					whatToChange_cursorPosition++;
					if (whatToChange_cursorPosition > 1) whatToChange_cursorPosition = 0;
					cursorChange();
				}
			}
		}

		if (hDown & KEY_A) {
			runSelection = true;
		}

		if ((hDown & KEY_TOUCH) && touch.px >= 71 && touch.px <= 248 && touch.py >= 91 && touch.py <= 136) {
			if (touch.px < 120) {
				whatToChange_cursorPosition = 0;
				cursorChange();
				runSelection = true;
			}
			if ((touch.px > 134) && (touch.px < 185) && highlightedGame==1) {
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
					Gui::setScreen(std::make_unique<MusicChange>(), true);
					break;
				case 2:
					Gui::setScreen(std::make_unique<EmblemChange>(), true);
					break;
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<GameSelect>(), true);
		}
	}
}