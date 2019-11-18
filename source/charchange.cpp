#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"

#include "ss4charnames.h"

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_WHAT_TO_DO = 2,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 3,	// Change character
};
//static int screenmode = 0;
extern int screenmodebuffer;

extern int highlightedGame;
extern bool saveWritten;

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

static int characterList_cursorPosition = 0;

static int characterShownFirst = 0;

void changeCharacter(void) {
	if (highlightedGame == 3) {
		readSS4Save();
	}

	cursorX = 256;

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, TRANSPARENT);
	C2D_TargetClear(bottom, TRANSPARENT);
	Gui::clearTextBufs();
	set_screen(top);

	Gui::sprite(sprites_blue_bg_idx, 0, 0);
	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	set_screen(bottom);
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	Draw_Text(8, 8, 0.50, BLACK, "Select the character you want to change.");

	int i2 = 48;
	if (highlightedGame == 3) {
		for (int i = characterShownFirst; i < characterShownFirst+3; i++) {
			if (i==0) {
				Draw_Text(48, i2, 0.65, BLACK, ss4PlayerName);
			} else {
				Draw_Text(48, i2, 0.65, BLACK, ss4CharacterNames[i]);
			}
			i2 += 48;
		}
	}

	Gui::sprite(sprites_button_shadow_idx, 5, 199);
	Gui::sprite(sprites_button_red_idx, 5, 195);
	Gui::sprite(sprites_arrow_back_idx, 19, 195);
	Gui::sprite(sprites_button_b_idx, 44, 218);

	drawCursor();

	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Draw_EndFrame();

	/*if (highlightedGame == 3 && !saveWritten) {
		readSS4Save();
		readSS4Character(0);
		if (ss4CharacterData.gender == 1) {
			ss4CharacterData.gender = 2;	// Male
		} else {
			ss4CharacterData.gender = 1;	// Female
		}
		writeSS4Character(0);
		writeSS4Save();
		saveWritten = true;
	}*/

	if(!fadein) {
		if (hDown & KEY_UP) {
			characterList_cursorPosition--;
			if (characterList_cursorPosition < 0) {
				characterList_cursorPosition = 0;
				characterShownFirst = 0;
			} else if (characterList_cursorPosition < characterShownFirst) {
				characterShownFirst--;
			}
		}
		if (hDown & KEY_DOWN) {
			characterList_cursorPosition++;
			if (characterList_cursorPosition > 0x20) {
				characterList_cursorPosition = 0x20;
				characterShownFirst = 0x20-2;
			} else if (characterList_cursorPosition >= 3) {
				characterShownFirst++;
			}
		}
		if (hDown & KEY_B) {
			screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
			fadeout = true;
		}
	}
}
