#include "mewtubeChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "savedata.h"

static const char* stageTitle[] = {
	"Open Plaza",
	"Open Plaza",
	"Outdoor Festival",
	"Outdoor Festival",
	"Concert Hall",
	"Concert Hall",
	"Charity Stage",
	"Charity Stage",
	"Live Music Club",
	"Live Music Club",
	"Celestial Hotel",
	"Celestial Hotel",
	"Alinatron On-Air",
	"Promo Video",
	"Cinema",
	"Concert Hall",
	"Concert Hall",
	"Open Plaza",
	"Open Plaza",
};

static const char* songTitle[] = {
	"Flying Shiny Day",
	"Ready Go!!",
	"Flying Shiny Day",
	"Ready Go!!",
	"Higher Higher",
	"To the Future",
	"Higher Higher",
	"To the Future",
	"Wings",
	"Beautiful",
	"Make Me Glow",
	"Glowing Moon",
	"Fight For Your Style",
	"Twinkle Fantasia",
	"Maria",
	"Dosukoi Koi Koi (Break It Down)",
	"Radiance",
	"Ring a Ding",
	"Bravo",
};

static const int peopleAmount[] = {
	1,
	1,
	3,
	1,
	3,
	1,
	2,
	2,
	1,
	3,
	3,
	1,
	1,
	1,
	1,
	3,
	3,
	3,
	3,
};

extern const char* getSS4CharName(u16 charId);

MewtubeChange::MewtubeChange() {
	readSS4Save();
}

void MewtubeChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	/*if (messageNo == 1) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Failed to apply music pack.");
	} else {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Successfully applied music pack.");
	}*/
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString((sysRegion==CFG_REGION_KOR ? 128 : 134), 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void MewtubeChange::Draw(void) const {
	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(255, 239, 123, 255));
	bool light = true;
	int x = 0;
	int width = 29;
	for (int i = 0; i < 14; i++) {
		if (light) {
			Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(255, 247, 173, 255));
		}
		light = !light;
		x += width;
		width = light ? 29 : 28;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(255, 251, 189, 255));
	bool dark = true;
	x = 0;
	width = 17;
	for (int i = 0; i < 14; i++) {
		if (dark) {
			Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(245, 239, 140, 255));
		}
		dark = !dark;
		x += width;
		width = dark ? 29 : 28;
	}
	Gui::Draw_Rect(0, 0, 320, 33, C2D_Color32(247, 235, 206, 255));
	Gui::Draw_Rect(0, 33, 320, 5, C2D_Color32(214, 158, 58, 255));

	cursorX = 248;
	cursorY = 64+(48*cursorPositionOnScreen[subScreenMode]);

	int i2 = 56;
	if (subScreenMode == 1) {
		Gui::DrawString(8, 8, 0.50, BLACK, "Select the character you want to change.");
		for (int i = 0; i < peopleAmount[cursorPosition]; i++) {
			u16 charId = getSS4MewtubeCharacterId(cursorPosition, 3+i);
			u16 orgCharId = getSS4MewtubeCharacterId(cursorPosition, i);
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			GFX::DrawSprite((getSS4CharacterGender(charId) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
			if (charId == orgCharId) {
				Gui::DrawString(64, i2, 0.65, BLACK, getSS4CharName(charId));
			} else {
				Gui::DrawString(64, i2-6, 0.60, BLACK, getSS4CharName(charId));
				Gui::DrawString(64, i2+12, 0.50, BLACK, getSS4CharName(orgCharId));
			}
			i2 += 48;
		}
	} else {
		Gui::DrawString(8, 8, 0.50, BLACK, "Select the video to change in You Diva mode.");
		for (int i = videoShownFirst; i < videoShownFirst+3; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2-8, 0.50, BLACK, stageTitle[i]);
			Gui::DrawString(32, i2+8, 0.60, BLACK, songTitle[i]);
			i2 += 48;
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


void MewtubeChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			showMessage = false;
		}
	} else if (subScreenMode == 1) {
		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 0;
		}
	} else {
		if (showCursor) {
			if (hDown & KEY_UP) {
				sndHighlight();
				cursorPosition--;
				cursorPositionOnScreen[0]--;
				if (cursorPosition < 0) {
					cursorPosition = 0;
					videoShownFirst = 0;
				} else if (cursorPosition < videoShownFirst) {
					videoShownFirst--;
				}
				if (cursorPositionOnScreen[0] < 0) {
					cursorPositionOnScreen[0] = 0;
				}
			}
			if (hDown & KEY_DOWN) {
				sndHighlight();
				cursorPosition++;
				cursorPositionOnScreen[0]++;
				if (cursorPosition > numberofVideos) {
					cursorPosition = numberofVideos;
					videoShownFirst = numberofVideos-2;
					if (videoShownFirst < 0) videoShownFirst = 0;
					if (cursorPositionOnScreen[0] > numberofVideos) {
						cursorPositionOnScreen[0] = numberofVideos;
					}
				} else if (cursorPosition > videoShownFirst+2) {
					videoShownFirst++;
				}
				if (cursorPositionOnScreen[0] > 2) {
					cursorPositionOnScreen[0] = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}
	}
}