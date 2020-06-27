#include "exiting.hpp"
#include "titleScreen.hpp"
#include "screenvars.h"
#include "settings.hpp"
#include "gameSelect.hpp"
#include "photoStudio.hpp"

extern u64 appID;
extern int orgHighlightedGame;
extern int titleSelection;
extern char verText[32];
extern bool exiting;
extern bool musicPlayStarted;

void titleScreen::drawCannotEditMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);

	Gui::DrawStringCentered(0, 92, 0.60, BLACK, "There is no save data");
	Gui::DrawStringCentered(0, 112, 0.60, BLACK, "that can be edited.");

	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void titleScreen::Draw(void) const {
	if (!musicPlayStarted) {
		extern void Play_Music();
		Play_Music();
		musicPlayStarted = true;
	}

	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	GFX::DrawSprite(sprites_logo_savvymanager_idx, 56, 58);

	Gui::DrawString(8, (cinemaWide ? 182 : 218)-(shiftBySubPixel ? 0.5f : 0), 0.50, BLACK, this->yearText);
	Gui::DrawString(328, (cinemaWide ? 182 : 218)-(shiftBySubPixel ? 0.5f : 0), 0.50, BLACK, verText);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);

	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	(titleSelection==0)
	? Gui::DrawStringCentered(0, 64, 0.75, BLACK, this->manageSaveText)
	: Gui::DrawStringCentered(0, 64, 0.50, BLACK, this->manageSaveText);
	(titleSelection==1)
	? Gui::DrawStringCentered(0, 96, 0.75, BLACK, this->photoStudioText)
	: Gui::DrawStringCentered(0, 96, 0.50, BLACK, this->photoStudioText);
	(titleSelection==2)
	? Gui::DrawStringCentered(0, 128, 0.75, BLACK, this->settingsText)
	: Gui::DrawStringCentered(0, 128, 0.50, BLACK, this->settingsText);

	if ((u32)appID == 0x3697300) {
		const int home_width = 144+16;
		const int home_x = (320-home_width)/2;
		GFX::DrawSprite(sprites_homeicon_idx, home_x, 218);
		Gui::DrawString(home_x+16, 218, 0.50, BLACK, this->returnToHomeText); // Draw HOME icon
	} else {
		Gui::DrawStringCentered(0, 218, 0.50, BLACK, this->exitText);
	}

	if (this->showMessage) {
		this->drawCannotEditMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void titleScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			this->showMessage = false;
		}

	} else {
		if (hDown & KEY_UP) {
			sndHighlight();
			titleSelection--;
			if (titleSelection < 0) titleSelection = 2;
		}
		if (hDown & KEY_DOWN) {
			sndHighlight();
			titleSelection++;
			if (titleSelection > 2) titleSelection = 0;
		}

		if (hDown & KEY_A) {
			sndSelect();
			switch (titleSelection) {
				case 0:
					Gui::setScreen(std::make_unique<GameSelect>(), true);
					break;
				case 1:
					orgHighlightedGame = highlightedGame;
					Gui::setScreen(std::make_unique<PhotoStudio>(), true);
					break;
				case 2:
					Gui::setScreen(std::make_unique<Settings>(), true);
					break;
			}
		}

		if (hDown & KEY_X) {
			sndBack();
			exiting = true;
			fadecolor = 0;
			Gui::setScreen(std::make_unique<Exiting>(), true);
		}
	}
}