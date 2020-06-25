#include "exiting.hpp"
#include "gameSelect.hpp"
#include "screenvars.h"
#include "titleScreen.hpp"
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
extern bool musicPlayStarted;

void GameSelect::drawCannotEditMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	if (this->messageNo == 2) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "This game's title ID");
		Gui::DrawStringCentered(0, 78, 0.60, BLACK, "is not known.");
		Gui::DrawStringCentered(0, 112, 0.60, BLACK, "As a result, the save data");
		Gui::DrawStringCentered(0, 132, 0.60, BLACK, "cannot be edited.");
	} else if (this->messageNo == 1) {
		if (highlightedGame == 0) {
			Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Save data not found.");
			Gui::DrawStringCentered(0, 90, 0.60, BLACK, /*highlightedGame==3 ? "Please back up the extra data using" :*/ "Please back it up using");
			Gui::DrawStringCentered(0, 110, 0.60, BLACK, "Checkpoint, and name the backup:");
			Gui::DrawStringCentered(0, 134, 0.60, BLACK, "SavvyManager");
		} else {
			Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Save data not found.");
		}
	} else {
		switch (sysRegion) {
			default:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Style Savvy's");
				break;
			case CFG_REGION_EUR:
			case CFG_REGION_AUS:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Style Boutique's");
				break;
			case CFG_REGION_JPN:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Girls Mode's");
			case CFG_REGION_KOR:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Girls Style's");
				break;
		}
		Gui::DrawStringCentered(0, 112, 0.60, BLACK, "save data yet.");
	}
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void GameSelect::Draw(void) const {
	if (!musicPlayStarted) {
		extern void Play_Music();
		Play_Music();
		musicPlayStarted = true;
	}

	Gui::ScreenDraw(Top);

	/*for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}*/

	switch(highlightedGame) {
		case 0:
		default:
			GFX::DrawGameShotSprite(gameShotSprites_title1_screenshot_idx, 0, (cinemaWide ? 8 : 0));
			break;
		case 1:
			GFX::DrawGameShotSprite(ss2Screenshot, 0, (cinemaWide ? 24 : 0));
			break;
		case 2:
			GFX::DrawGameShotSprite(gameShotSprites_title3_screenshot_idx, 0, (cinemaWide ? 24 : 0));
			break;
		case 3:
			GFX::DrawGameShotSprite(gameShotSprites_title4_screenshot_idx, 0, (cinemaWide ? 32 : 0));
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	Gui::ScreenDraw(Bottom);

	switch(highlightedGame) {
		case 0:
		default:
			/*Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}*/
			GFX::DrawGameBgSprite((sysRegion==CFG_REGION_EUR || sysRegion==CFG_REGION_AUS) ? gameBgSprites_title1_bgE_idx : gameBgSprites_title1_bg_idx, 0, 0, 2, 2);
			GFX::DrawGameSelSprite(ss1Logo, ss1LogoXpos, 56);
			break;
		case 1:
			GFX::DrawGameBgSprite(gameBgSprites_title2_bg_idx, 0, 0);
			GFX::DrawGameSelSprite(ss2Logo, ssLogoXpos, 56);
			break;
		case 2:
			GFX::DrawGameBgSprite(gameBgSprites_title3_bg_idx, 0, 0);
			GFX::DrawGameSelSprite(ss3Logo, 0, 56);
			break;
		case 3:
			GFX::DrawGameBgSprite((sysRegion==CFG_REGION_EUR || sysRegion==CFG_REGION_AUS) ? gameBgSprites_title4_bgE_idx : gameBgSprites_title4_bg_idx, 0, 0);
			GFX::DrawGameSelSprite(ss4Logo, ssLogoXpos, 56);
			break;
	}
	Gui::DrawString(8, 8, 0.50, BLACK, "Select a game to manage its save data.");
	Gui::DrawString(8, 112, 0.55, BLACK, "<");
	Gui::DrawString(304, 112, 0.55, BLACK, ">");

	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);

	if (this->showMessage) {
		this->drawCannotEditMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void GameSelect::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			this->showMessage = false;
		}

	} else {
		if ((hDown & KEY_LEFT) || ((hDown & KEY_TOUCH) && touch.px >= 0 && touch.px < 32 && touch.py >= 104 && touch.py < 104+32)) {
			sndHighlight();
			highlightedGame--;
			if (highlightedGame < 0) highlightedGame = (sysRegion==CFG_REGION_KOR ? 1 : 3);
		} else if ((hDown & KEY_RIGHT) || ((hDown & KEY_TOUCH) && touch.px >= 320-32 && touch.px < 320 && touch.py >= 104 && touch.py < 104+32)) {
			sndHighlight();
			highlightedGame++;
			if (highlightedGame > (sysRegion==CFG_REGION_KOR ? 1 : 3)) highlightedGame = 0;
		}

		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 32 && touch.px < 320-32 && touch.py >= 56 && touch.py < 56+128)) {
		  if (highlightedGame == 0) {
			sndBack();
			this->messageNo = 0;
			this->showMessage = true;
		  } else if (highlightedGame == 1 && sysRegion == CFG_REGION_KOR) {
			sndBack();
			this->messageNo = 2;
			this->showMessage = true;
		  } else if ((highlightedGame == 1 && ss2SaveFound)
		  || (highlightedGame == 2 && ss3SaveFound)
		  || (highlightedGame == 3 && ss4SaveFound))
		  {
			sndSelect();
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		 } else {
			sndBack();
			this->messageNo = 1;
			this->showMessage = true;
		 }
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<titleScreen>(), true);
		}
	}
}