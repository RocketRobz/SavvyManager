#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "common.hpp"
#include "savedata.h"
//#include "settings.h"
#include "file_browse.h"

extern std::string currentMusicPack;
extern void saveSettings(void);

extern void sndSelect(void);
extern void sndBack(void);
extern void sndHighlight(void);

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_WHAT_TO_DO = 2,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 3,	// Change character
	SCREEN_MODE_CHANGE_MUSIC = 4,		// Change music
	SCREEN_MODE_CHANGE_EMBLEM = 5,		// Change emblem
};
//static int screenmode = 0;
extern int screenmodebuffer;

//static int subScreenMode = 0;
/*
*/

extern u8 sysRegion;
extern int highlightedGame;

extern int fadealpha;
extern int fadecolor;
extern bool fadein;
extern bool fadeout;

extern float bg_xPos;
extern float bg_yPos;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

extern u32 hDown;
extern touchPosition touch;
extern bool touchingBackButton(void);

static int cursorPosition = 0;
static int cursorPositionOnScreen = 0;

static int musicPackShownFirst = 0;

static bool modeInited = false;

static bool showMessage = false;
static int messageNo = 0;

static void drawMsg(void) {
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

void changeMusicGraphics(void) {
	if (!modeInited) {
		getMusicPackContents();
		modeInited = true;
	}

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	Gui::clearTextBufs();
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

	cursorX = 248;
	cursorY = 64+(48*cursorPositionOnScreen);

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

	GFX::drawCursor();

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	C3D_FrameEnd(0);
}

void changeMusic(void) {
	if (!fadein && !fadeout) {
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
			screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
			fadeout = true;
		}
		}
	}
}
