#include "file_browse.h"
#include "musicChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

extern void saveSettings(void);

MusicChange::MusicChange() {
	if (!modeInited) {
		getMusicPackContents();
		modeInited = true;
	}
}

void MusicChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	if (messageNo == 1) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Failed to apply music pack.");
	} else {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Successfully applied music pack.");
	}
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void MusicChange::Draw(void) const {
	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	Gui::DrawString(8, 206, 0.50, BLACK, "Current music pack:");
	Gui::DrawString(8, 220, 0.50, BLACK, (currentMusicPack=="" ? "Original" : currentMusicPack.c_str()));

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	this->cursorX = 248;
	this->cursorY = 64+(48*cursorPositionOnScreen);

	Gui::DrawString(8, 8, 0.50, BLACK, "Select the music pack you want to use.");

	int i2 = 48;
	for (int i = musicPackShownFirst; i < musicPackShownFirst+3; i++) {
		if (i > numberOfMusicPacks) break;
	
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		if (i == 0) {
			Gui::DrawString(32, i2, 0.65, BLACK, "Original/Revert");
		} else {
			Gui::DrawString(32, i2, 0.65, BLACK, getMusicPackName(i-1));
		}
		i2 += 48;
	}

	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);

	GFX::drawCursor(this->cursorX, this->cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void MusicChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			showMessage = false;
		}
	} else {
		if (showCursor) {
			if (hDown & KEY_UP) {
				sndHighlight();
				cursorPosition--;
				cursorPositionOnScreen--;
				if (cursorPosition < 0) {
					cursorPosition = 0;
					musicPackShownFirst = 0;
				} else if (cursorPosition < musicPackShownFirst) {
					musicPackShownFirst--;
				}
				if (cursorPositionOnScreen < 0) {
					cursorPositionOnScreen = 0;
				}
			}
			if (hDown & KEY_DOWN) {
				sndHighlight();
				cursorPosition++;
				cursorPositionOnScreen++;
				if (cursorPosition > numberOfMusicPacks) {
					cursorPosition = numberOfMusicPacks;
					musicPackShownFirst = numberOfMusicPacks-2;
						if (musicPackShownFirst < 0) musicPackShownFirst = 0;
						if (cursorPositionOnScreen > numberOfMusicPacks) {
							cursorPositionOnScreen = numberOfMusicPacks;
						}
					} else if (cursorPosition > musicPackShownFirst+2) {
						musicPackShownFirst++;
					}
					if (cursorPositionOnScreen > 2) {
						cursorPositionOnScreen = 2;
					}
				}
			}
		if ((hDown & KEY_A) && (cursorPosition <= numberOfMusicPacks)) {
			sndSelect();
			char prevMusicPackPath[256];
			char musicPackPath[256];
			sprintf(prevMusicPackPath, "sdmc:/3ds/SavvyManager/SS2/musicPacks/%s", currentMusicPack.c_str());
			if (cursorPosition > 0) {
				sprintf(musicPackPath, "sdmc:/3ds/SavvyManager/SS2/musicPacks/%s", getMusicPackName(cursorPosition-1));
			}
			const char* romfsStreamPath = "sdmc:/luma/titles/00040000000A9100/romfs/Common/Sound/stream";
			switch (sysRegion) {
					case CFG_REGION_EUR:
					case CFG_REGION_AUS:
						romfsStreamPath = "sdmc:/luma/titles/00040000000A9000/romfs/Common/Sound/stream";
						break;
					case CFG_REGION_JPN:
						romfsStreamPath = "sdmc:/luma/titles/000400000005D100/romfs/Common/Sound/stream";
						break;
					default:
						break;
				}
				rename(romfsStreamPath, prevMusicPackPath);
				if (cursorPosition==0 || rename(musicPackPath, romfsStreamPath) == 0) {
					messageNo = 0;
				} else {
					messageNo = 1;
				}
				showMessage = true;
				currentMusicPack = (cursorPosition==0 ? "" : getMusicPackName(cursorPosition-1));
				saveSettings();
				modeInited = false;
			}
			
		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}
	}
}