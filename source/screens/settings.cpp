#include "gameSelect.hpp"
#include "settings.hpp"
#include "screenvars.h"

char txt_frameRate[24];

void Settings::Draw(void) const {
	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
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

	this->cursorX = 248;
	this->cursorY = 64+(48*cursorPositionOnScreen);

	Gui::DrawStringCentered(0, 8, 0.55, BLACK, "Settings");

	int i2 = 48;
	for (int i = settingShownFirst; i < settingShownFirst+3; i++) {
		if (i > numberOfSettings) break;
	
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		switch (i) {
			case 0:
				sprintf(txt_frameRate, "Frame Rate: %iFPS", iFps);
				Gui::DrawString(32, i2, 0.65, BLACK, txt_frameRate);
				break;
			default:
				break;
		}
		i2 += 48;
	}

	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);

	GFX::drawCursor(cursorX, cursorY);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}

void Settings::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
	/*if (showCursor) {
		if (hDown & KEY_UP) {
			sndHighlight();
			cursorPosition--;
			cursorPositionOnScreen--;
			if (cursorPosition < 0) {
				cursorPosition = 0;
				settingShownFirst = 0;
			} else if (cursorPosition < settingShownFirst) {
				settingShownFirst--;
			}
			if (cursorPositionOnScreen < 0) {
				cursorPositionOnScreen = 0;
			}
		}

		if (hDown & KEY_DOWN) {
			sndHighlight();
			cursorPosition++;
			cursorPositionOnScreen++;
			if (cursorPosition > numberOfSettings) {
				cursorPosition = numberOfSettings;
				settingShownFirst = numberOfSettings-2;
				if (settingShownFirst < 0) settingShownFirst = 0;
				if (cursorPositionOnScreen > numberOfSettings) {
					cursorPositionOnScreen = numberOfSettings;
				}
			} else if (cursorPosition > settingShownFirst+2) {
				settingShownFirst++;
			}
			if (cursorPositionOnScreen > 2) {
				cursorPositionOnScreen = 2;
			}
		}
	}*/

	if ((hDown & KEY_LEFT) && (cursorPosition <= numberOfSettings)) {
		sndSelect();
		switch (cursorPosition) {
			case 0:
					 if (iFps==30) iFps = 24;
				else if (iFps==24) iFps = 60;
				else if (iFps==60) iFps = 30;
				C3D_FrameRate(iFps);
				break;
		}
	}

	if (((hDown & KEY_RIGHT) || (hDown & KEY_A)) && (cursorPosition <= numberOfSettings)) {
		sndSelect();
		switch (cursorPosition) {
			case 0:
					 if (iFps==60) iFps = 24;
				else if (iFps==24) iFps = 30;
				else if (iFps==30) iFps = 60;
				C3D_FrameRate(iFps);
				break;
		}
	}

	if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
		sndBack();
		Gui::setScreen(std::make_unique<GameSelect>(), true);
	}
}