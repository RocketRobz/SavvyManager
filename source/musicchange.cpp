#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"
#include "file_browse.hpp"

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

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
};
//static int screenmode = 0;
extern int screenmodebuffer;

static int subScreenMode = 0;
/*
*/

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

extern void drawCursor(void);

extern u32 hDown;

static int cursorPosition = 0;
static int cursorPositionOnScreen = 0;

static int musicPackShownFirst = 0;

static bool modeInited = false;

static bool showMessage = false;
static int messageNo = 0;

static void drawMsg(void) {
	Gui::spriteScale(sprites_msg_idx, 0, 0, 2, 1);
	Gui::spriteScale(sprites_msg_idx, 160, 0, -2, 1);
	/*if (messageNo == 3) {
		Draw_Text(32, 84, 0.60, BLACK, "Exported character does not exist.");
	} else if (messageNo == 2) {
		Draw_Text(32, 48, 0.60, BLACK, "Character exported successfully.");
		Draw_Text(32, 84, 0.60, BLACK, "You can go to \"Import Characters\"");
		Draw_Text(32, 104, 0.60, BLACK, "and restore the exported character");
		Draw_Text(32, 124, 0.60, BLACK, "at any time.");
	} else if (messageNo == 1) {
		Draw_Text(32, 48, 0.60, BLACK, chararacterImported);
		Draw_Text(32, 84, 0.60, BLACK, "Please restore \"SavvyManager\"");
		Draw_Text(32, 104, 0.60, BLACK, "data for your game in Checkpoint,");
		Draw_Text(32, 124, 0.60, BLACK, "for the change to take effect.");
	} else {
		Draw_Text(32, 84, 0.60, BLACK, "This feature is not available yet.");
		//Draw_Text(32, 104, 0.60, BLACK, "yet.");
	}*/
	Draw_Text(32, 160, 0.65, BLACK, " OK");
}

void changeMusic(void) {
	if (!modeInited) {
		getMusicPackContents();
		modeInited = true;
	}

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, TRANSPARENT);
	C2D_TargetClear(bottom, TRANSPARENT);
	Gui::clearTextBufs();
	set_screen(top);

	Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	cursorX = 256;
	cursorY = 64+(48*cursorPositionOnScreen);

	Draw_Text(8, 8, 0.50, BLACK, "Select the music pack you want to use.");

	int i2 = 48;
	for (int i = musicPackShownFirst; i < musicPackShownFirst+3; i++) {
		if (i > numberOfMusicPacks) break;
	
		if (i == 0) {
			Draw_Text(64, i2, 0.65, BLACK, "Original/Revert");
		} else {
			Draw_Text(64, i2, 0.65, BLACK, getMusicPackName(i-1));
		}
		i2 += 48;
	}

	Gui::sprite(sprites_button_shadow_idx, 5, 199);
	Gui::sprite(sprites_button_red_idx, 5, 195);
	Gui::sprite(sprites_arrow_back_idx, 19, 195);
	Gui::sprite(sprites_button_b_idx, 44, 218);

	drawCursor();

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Draw_EndFrame();

	if (!fadein && !fadeout) {
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
		if (hDown & KEY_B) {
			sndBack();
			screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
			fadeout = true;
		}
	}
}
