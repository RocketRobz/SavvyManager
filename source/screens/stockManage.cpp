#include "stockManage.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "savedata.h"

extern bool ss3SaveFound;

static const char* secondOptionText(void) {
	switch (sysRegion) {
		default:
			return "Transfer from Fashion Forward";
		case CFG_REGION_JPN:
			return "Transfer from Kirakira * Code";
		case CFG_REGION_KOR:
			return "Transfer from Glitter * Code";
	}
}

static const char* optionList[] = {
	"Add all Fashion Items",
	secondOptionText(),
};

StockManage::StockManage() {
	(highlightedGame == 3) ? readSS4Save() : readSS3Save();
}

void StockManage::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(messageNo == 1 ? sprites_icon_question_idx : sprites_icon_msg_idx, 132, -2);

	if (messageNo == 1) {
		if (cursorPosition == 1) {
			Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Fashion items owned in the");
			Gui::DrawStringCentered(0, 78, 0.60, BLACK, "previous game will be added");
			Gui::DrawStringCentered(0, 98, 0.60, BLACK, "to your boutique and wardrobe.");
		} else {
			Gui::DrawStringCentered(0, 68, 0.60, BLACK, "All fashion items will be added");
			Gui::DrawStringCentered(0, 88, 0.60, BLACK, "to your boutique and wardrobe.");
		}
		Gui::DrawStringCentered(0, 144, 0.60, BLACK, "Is this OK?");
	} else {
		if (cursorPosition == 1) {
			Gui::DrawStringCentered(0, 68, 0.60, BLACK, "Fashion items have been transferred.");
			Gui::DrawStringCentered(0, 114, 0.60, BLACK, "Some items have not been");
			Gui::DrawStringCentered(0, 134, 0.60, BLACK, "transferred, due to them");
			Gui::DrawStringCentered(0, 164, 0.60, BLACK, "being removed from this game.");
		} else {
			Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Fashion items have been added.");
		}
	}

	if (messageNo == 1) {
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 52, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 53, 188);
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 176, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 177, 188);
		Gui::DrawString(72, 196, 0.70, MSG_BUTTONTEXT, " No");
		Gui::DrawString(196, 196, 0.70, MSG_BUTTONTEXT, " Yes");
	} else {
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
		Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
	}
}

void StockManage::Draw(void) const {
	Gui::ScreenDraw(Top);

	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(123, 239, 123, 255));
		bool light = true;
		int x = 0;
		int width = 29;
		for (int i = 0; i < 14; i++) {
			if (light) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(173, 247, 173, 255));
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

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(189, 251, 189, 255));
		bool dark = true;
		int x = 0;
		int width = 17;
		for (int i = 0; i < 14; i++) {
			if (dark) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(140, 239, 140, 255));
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

	cursorX = 248;
	cursorY = 64+(48*cursorPosition);

	int i2 = (highlightedGame == 3 ? 56 : 48);
	Gui::DrawString(8, 8, 0.50, BLACK, "What do you want to do with your stock?");
	for (int i = 0; i < 2; i++) {
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.50, ((i == 0) || (i == 1 && ss3SaveFound)) ? BLACK : HALF_BLACK, optionList[i]);
		i2 += 48;
		if (highlightedGame != 3) {
			break;
		}
	}

	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);

	GFX::drawCursor(cursorX, cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void StockManage::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if (messageNo == 1) {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 176 && touch.px < 176+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				bool flagModified = false;
				if (cursorPosition == 1) {
					for (int i = 0; i < 15500; i++) {
						u16 flag = readSS3FashionOwnFlag(i);
						if (flag == 0x0101 || flag == 0x0102 || flag == 0x0202 || flag == 0x0201) {
							writeSS4FashionOwnFlag(i, flag);
							flagModified = true;
						}
					}
				} else {
					for (int i = 0; i < ((highlightedGame == 3) ? 30304 : 15500); i++) {
						(highlightedGame == 3) ? writeSS4FashionOwnFlag(i, 0xFFFF) : writeSS3FashionOwnFlag(i, 0xFFFF);
					}
					flagModified = true;
				}
				if (flagModified) {
					(highlightedGame == 3) ? writeSS4FashionOwnFlagsToSave() : writeSS3FashionOwnFlagsToSave();
				}
				messageNo = 0;
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndBack();
				showMessage = false;
			}
		} else {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				showMessage = false;
			}
		}
	} else {
		if (showCursor && (highlightedGame == 3)) {
			if (hDownRepeat & KEY_UP) {
				sndHighlight();
				cursorPosition--;
				if (cursorPosition < 0) {
					cursorPosition = 0;
				}
			}
			if (hDownRepeat & KEY_DOWN) {
				sndHighlight();
				cursorPosition++;
				if (cursorPosition > numberofOptions) {
					cursorPosition = numberofOptions;
				}
			}
		}

		if (hDown & KEY_A) {
			if (cursorPosition == 1) {
				if (ss3SaveFound) {
					readSS3Save();
					sndSelect();
					messageNo = 1;
					showMessage = true;
				} else {
					sndBack();
				}
			} else {
				sndSelect();
				messageNo = 1;
				showMessage = true;
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}
	}
}