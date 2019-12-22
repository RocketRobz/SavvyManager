#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"
#include "file_browse.h"

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
	SCREEN_MODE_CHANGE_EMBLEM = 5,		// Change emblem
};
//static int screenmode = 0;
extern int screenmodebuffer;

//static int subScreenMode = 0;
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

static int totalEmblems = 0;

/*static u32 emblemPalette[16] = 
{
	C2D_Color32(0, 0, 0, 255),
	C2D_Color32(16, 16, 16, 255),
	C2D_Color32(32, 32, 32, 255),
	C2D_Color32(48, 48, 48, 255),
	C2D_Color32(64, 64, 64, 255),
	C2D_Color32(80, 80, 80, 255),
	C2D_Color32(96, 96, 96, 255),
	C2D_Color32(112, 112, 112, 255),
	C2D_Color32(128, 128, 128, 255),
	C2D_Color32(144, 144, 144, 255),
	C2D_Color32(160, 160, 160, 255),
	C2D_Color32(176, 176, 176, 255),
	C2D_Color32(192, 192, 192, 255),
	C2D_Color32(208, 208, 208, 255),
	C2D_Color32(224, 224, 224, 255),
	C2D_Color32(240, 240, 240, 255)
};

static u32 emblemPixel(int pixel, bool secondPixel) {
	pixel = pixel/2;
	if (secondPixel) {
		return emblemPalette[emblemData.sprite[pixel & 0x0F]];
	} else {
		return emblemPalette[emblemData.sprite[pixel & 0xF0]];
	}
}

static u32 emblemImage[64*64];

static void renderEmblem(void) {
	bool secondPixel = false;
	for (int i = 0; i < 64*64; i++) {
		emblemImage[i] = emblemPixel(i, secondPixel);
		secondPixel = !secondPixel;
	}
}

static void drawEmblem(int x, int y) {
	for (int h = 0; h < 64; h++) {
		for (int w = 0; w < 64; w++) {
			Draw_Rect(x+w, y+h, 1, 1, emblemImage[w*h]);
		}
	}
}*/

static bool modeInited = false;

static bool showMessage = false;
static int messageNo = 0;

static void drawMsg(void) {
	Gui::spriteScale(sprites_msg_idx, 0, 0, 2, 1);
	Gui::spriteScale(sprites_msg_idx, 160, 0, -2, 1);
	if (messageNo == 1) {
		Draw_Text(32, 84, 0.60, BLACK, "Failed to apply emblem.");
	} else {
		Draw_Text(32, 84, 0.60, BLACK, "Successfully applied emblem.");
	}
	Draw_Text(32, 160, 0.65, BLACK, " OK");
}

void changeEmblem(void) {
	if (!modeInited) {
		//(highlightedGame==3) ? readSS4Emblem(0) : readSS3Emblem();
		//renderEmblem();
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
	Gui::spriteScale(sprites_emblem_back_idx, 100, 20, 2, 2);
	//drawEmblem(128, 32);

	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	cursorX = 256;
	cursorY = 64+(48*cursorPosition);

	Draw_Text(8, 8, 0.50, BLACK, "Select the emblem to change.");

	int i2 = 48;
	char emblemText[10];
	for (int i = 0; i <= totalEmblems; i++) {
		if (highlightedGame == 2) {
			sprintf(emblemText, "Emblem");
		} else {
			sprintf(emblemText, "Emblem %i", i);
		}
		Draw_Text(64, i2, 0.65, BLACK, emblemText);
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
		if (showMessage) {
			if (hDown & KEY_A) {
				sndSelect();
				showMessage = false;
			}
		} else {
		if (showCursor) {
			if (hDown & KEY_UP) {
				sndHighlight();
				cursorPosition--;
				if (cursorPosition < 0) {
					cursorPosition = 0;
				}
			}
			if (hDown & KEY_DOWN) {
				sndHighlight();
				cursorPosition++;
				if (cursorPosition > totalEmblems) {
					cursorPosition = totalEmblems;
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
}
