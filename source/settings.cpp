#include <3ds.h>
#include <string>

#define settingsIni "sdmc:/3ds/SavvyManager/settings.ini"

#include "common.hpp"
#include "screenMode.h"
#include "inifile.h"

int studioBg = 0;
int iFps = 60;
std::string currentMusicPack = "";

static const char* studioBgName(void) {
	switch (studioBg) {
		case 0:
		default:
			return "Blue";
		case 1:
			return "Lovers Bell";
	}

	return "null";
}

extern void sndSelect(void);
extern void sndBack(void);
extern void sndHighlight(void);

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

static bool displayStudioBg = true;

#define numberOfSettings 1

static int cursorPosition = 0;
static int cursorPositionOnScreen = 0;

static int settingShownFirst = 0;

//static bool modeInited = false;

static char txt_studioBG[32];
static char txt_frameRate[24];

void settingsMenuGraphics(void) {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	Gui::clearTextBufs();
	Gui::ScreenDraw(Top);

	if (displayStudioBg) {
		GFX::showBgSprite(0);
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}

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

	Gui::DrawStringCentered(0, 8, 0.55, BLACK, "Settings");

	int i2 = 48;
	for (int i = settingShownFirst; i < settingShownFirst+3; i++) {
		if (i > numberOfSettings) break;
	
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		switch (i) {
			case 0:
				sprintf(txt_studioBG, "Studio BG: %s", studioBgName());
				Gui::DrawString(32, i2, 0.65, BLACK, txt_studioBG);
				break;
			case 1:
				sprintf(txt_frameRate, "Frame Rate: %i", iFps);
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

	GFX::drawCursor();

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	C3D_FrameEnd(0);
}

void loadSettings(void) {
	CIniFile settingsini(settingsIni);

	studioBg = settingsini.GetInt("SAVVY-MANAGER", "STUDIO_BG", studioBg);
	iFps = settingsini.GetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);

	currentMusicPack = settingsini.GetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);
}

void saveSettings(void) {
	CIniFile settingsini(settingsIni);

	settingsini.SetInt("SAVVY-MANAGER", "STUDIO_BG", studioBg);
	settingsini.SetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);

	settingsini.SetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);

	settingsini.SaveIniFileModified(settingsIni);
}

void settingsMenu(void) {
	if (!fadein && !fadeout) {
		if (showCursor) {
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
		}
		if ((hDown & KEY_A) && (cursorPosition <= numberOfSettings)) {
			sndSelect();
			switch (cursorPosition) {
				case 0:
					studioBg++;
					if (studioBg > 1) studioBg = 0;
					displayStudioBg = false;
					gspWaitForVBlank();
					GFX::loadBgSprite();
					displayStudioBg = true;
					break;
				case 1:
					      if (iFps==60) iFps = 24;
					else if (iFps==24) iFps = 30;
					else if (iFps==30) iFps = 60;
					C3D_FrameRate(iFps);
					break;
			}
		}
		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			screenmodebuffer = SCREEN_MODE_GAME_SELECT;
			fadeout = true;
		}
	}
}
