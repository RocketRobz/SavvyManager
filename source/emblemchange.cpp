#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"
#include "file_browse.h"

#include "import_emblemnames.h"

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

static int subScreenMode = 0;
/*
	0: Emblem list
	1: What to change
	2: Import emblem
*/

static int importPage = 0;

static char embFilePath[256];

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
extern touchPosition touch;
extern bool touchingBackButton(void);

static bool displayNothing = false;

static int cursorPosition = 0;
static int emblemChangeMenu_cursorPosition = 0;
static int importEmblemList_cursorPosition = 0;
static int importEmblemList_cursorPositionOnScreen = 0;

static int totalEmblems = 0;

static int import_emblemShownFirst = 0;

/*static u32 emblemPalette[16] = 
{
	C2D_Color32(0, 0, 0, 0),
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

static int getPalNumber(u8 byte, bool secondPixel) {
	if (secondPixel) {
		if ((byte & 0x0F) == 0x0) {
			return 0;
		} else if ((byte & 0x0F) == 0x1) {
			return 1;
		} else if ((byte & 0x0F) == 0x2) {
			return 2;
		} else if ((byte & 0x0F) == 0x3) {
			return 3;
		} else if ((byte & 0x0F) == 0x4) {
			return 4;
		} else if ((byte & 0x0F) == 0x5) {
			return 5;
		} else if ((byte & 0x0F) == 0x6) {
			return 6;
		} else if ((byte & 0x0F) == 0x7) {
			return 7;
		} else if ((byte & 0x0F) == 0x8) {
			return 8;
		} else if ((byte & 0x0F) == 0x9) {
			return 9;
		} else if ((byte & 0x0F) == 0xA) {
			return 10;
		} else if ((byte & 0x0F) == 0xB) {
			return 11;
		} else if ((byte & 0x0F) == 0xC) {
			return 12;
		} else if ((byte & 0x0F) == 0xD) {
			return 13;
		} else if ((byte & 0x0F) == 0xE) {
			return 14;
		} else if ((byte & 0x0F) == 0xF) {
			return 15;
		}
	} else {
		if (byte >= 0x00 && byte < 0x10) {
			return 0;
		} else if (byte >= 0x10 && byte < 0x20) {
			return 1;
		} else if (byte >= 0x20 && byte < 0x30) {
			return 2;
		} else if (byte >= 0x30 && byte < 0x40) {
			return 3;
		} else if (byte >= 0x40 && byte < 0x50) {
			return 4;
		} else if (byte >= 0x50 && byte < 0x60) {
			return 5;
		} else if (byte >= 0x60 && byte < 0x70) {
			return 6;
		} else if (byte >= 0x70 && byte < 0x80) {
			return 7;
		} else if (byte >= 0x80 && byte < 0x90) {
			return 8;
		} else if (byte >= 0x90 && byte < 0xA0) {
			return 9;
		} else if (byte >= 0xA0 && byte < 0xB0) {
			return 10;
		} else if (byte >= 0xB0 && byte < 0xC0) {
			return 11;
		} else if (byte >= 0xC0 && byte < 0xD0) {
			return 12;
		} else if (byte >= 0xD0 && byte < 0xE0) {
			return 13;
		} else if (byte >= 0xE0 && byte < 0xF0) {
			return 14;
		} else if (byte >= 0xF0 && byte < 0x100) {
			return 15;
		}
	}
	return 0;
}

static u32 emblemPixel(int pixel, bool secondPixel) {
	pixel = pixel/2;
	return emblemPalette[getPalNumber(emblemData.sprite[pixel], secondPixel)];
}

static u32 emblemImage[64*64];

static void renderEmblem(void) {
	bool secondPixel = false;
	for (int i = 0; i < 64*64; i++) {
		emblemImage[i] = emblemPixel(i, secondPixel);
		secondPixel = !secondPixel;
	}
}

static bool emblemHalf = false;

static void drawEmblem(int x, int y, bool big) {
	for (int h = (emblemHalf ? 32: 0); h < (emblemHalf ? 64: 32); h++) {
		for (int w = 0; w < 64; w++) {
			Draw_Rect(x+(w*(big*2)), y+(h*(big*2)), 1+big, 1+big, emblemImage[(h*64)+w]);
		}
	}
	emblemHalf = !emblemHalf;
}*/

static bool modeInited = false;

static bool showMessage = false;
static int messageNo = 0;

static char emblemImported[48];

static void drawMsg(void) {
	Gui::sprite(sprites_msg_idx, 0, 8, 2, 1);
	Gui::sprite(sprites_msg_idx, 160, 8, -2, 1);
	Gui::sprite(sprites_icon_msg_idx, 132, -2);
	if (messageNo == 3) {
		Gui::DrawString(32, 94, 0.60, BLACK, "Failed to import emblem.");
	} else if (messageNo == 2) {
		Gui::DrawString(32, 58, 0.60, BLACK, "Emblem exported successfully.");
		Gui::DrawString(32, 94, 0.60, BLACK, "You can go to \"Import Emblems\"");
		Gui::DrawString(32, 114, 0.60, BLACK, "and restore the exported emblem");
		Gui::DrawString(32, 134, 0.60, BLACK, "at any time.");
	} else if (messageNo == 1) {
		Gui::DrawString(32, 58, 0.60, BLACK, emblemImported);
		Gui::DrawString(32, 94, 0.60, BLACK, "Please restore \"SavvyManager\"");
		Gui::DrawString(32, 114, 0.60, BLACK, "data for your game in Checkpoint,");
		Gui::DrawString(32, 134, 0.60, BLACK, "for the change to take effect.");
	} else {
		Gui::DrawString(32, 94, 0.60, BLACK, "This feature is not available yet.");
		//Gui::DrawString(32, 104, 0.60, BLACK, "yet.");
	}
	Gui::sprite(sprites_button_msg_shadow_idx, 114, 197);
	Gui::sprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void changeEmblemGraphics(void) {
	if (subScreenMode == 2) {
		if (importPage == 1) {
			totalEmblems = numberOfExportedEmblems-1;
		} else {
			totalEmblems = 10;
		}
	} else if (highlightedGame == 3) {
		totalEmblems = 2;
		readSS4Save();
		//readSS4Emblem(cursorPosition);
	} else {
		totalEmblems = 0;
		readSS3Save();
		//readSS3Emblem();
	}

	if (!modeInited) {
		//renderEmblem();
		modeInited = true;
	}

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, TRANSPARENT);
	C2D_TargetClear(bottom, TRANSPARENT);
	Gui::clearTextBufs();
	Gui::setDraw(top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	Gui::sprite(sprites_emblem_back_idx, 100, 20, 2, 2);
	//drawEmblem(136, 56, true);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	Gui::setDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	char emblemText[32];
	cursorX = 248;
	if (subScreenMode == 2) {
		cursorY = 64+(48*importEmblemList_cursorPositionOnScreen);

		// Game name
		switch (importPage) {
			case 1:
				Gui::DrawString(32, 8, 0.50, BLACK, "Your emblem files");
				break;
			case 0:
				Gui::DrawString(32, 8, 0.50, BLACK, "Savvy Manager");
				break;
		}
		Gui::DrawString(8, 8, 0.50, BLACK, "<");
		Gui::DrawString(304, 8, 0.50, BLACK, ">");

	  if (!displayNothing) {
		int i2 = 48;
		for (int i = import_emblemShownFirst; i < import_emblemShownFirst+3; i++) {
			if (importPage == 1) {
				if (i >= numberOfExportedEmblems) break;
			} else {
				if (i > totalEmblems) break;
			}
			Gui::sprite(sprites_item_button_idx, 16, i2-20);
			if (importPage == 1) {
				Gui::DrawString(32, i2, 0.65, BLACK, getExportedEmblemName(i));
			} else {
				Gui::DrawString(32, i2, 0.65, BLACK, import_emblemNames[i]);
			}
			i2 += 48;
		}
	  }
	} else if (subScreenMode == 1) {
		if (highlightedGame == 2) {
			sprintf(emblemText, "Emblem");
		} else {
			sprintf(emblemText, "Emblem %i", cursorPosition+1);
		}

		cursorY = 64+(48*emblemChangeMenu_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, emblemText);

		int i2 = 48;
		Gui::sprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Import emblem");
		i2 += 48;
		Gui::sprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Export emblem");
	} else {
		cursorY = 64+(48*cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the emblem to change.");

		int i2 = 48;
		for (int i = 0; i <= totalEmblems; i++) {
			Gui::sprite(sprites_item_button_idx, 16, i2-20);
			if (highlightedGame == 2) {
				sprintf(emblemText, "Emblem");
			} else {
				sprintf(emblemText, "Emblem %i", i+1);
			}
			Gui::DrawString(32, i2, 0.65, BLACK, emblemText);
			i2 += 48;
		}
	}

	Gui::sprite(sprites_button_shadow_idx, 5, 199);
	Gui::sprite(sprites_button_red_idx, 5, 195);
	Gui::sprite(sprites_arrow_back_idx, 19, 195);
	Gui::sprite(sprites_button_b_idx, 44, 218);

	drawCursor();

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	C3D_FrameEnd(0);
}

void changeEmblem(void) {
	if (!fadein && !fadeout) {
		if (showMessage) {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				showMessage = false;
			}
		} else if (subScreenMode == 2) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					importEmblemList_cursorPosition--;
					importEmblemList_cursorPositionOnScreen--;
					if (importEmblemList_cursorPosition < 0) {
						importEmblemList_cursorPosition = 0;
						import_emblemShownFirst = 0;
					} else if (importEmblemList_cursorPosition < import_emblemShownFirst) {
						import_emblemShownFirst--;
					}
					if (importEmblemList_cursorPositionOnScreen < 0) {
						importEmblemList_cursorPositionOnScreen = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					importEmblemList_cursorPosition++;
					importEmblemList_cursorPositionOnScreen++;
					if (importEmblemList_cursorPosition > totalEmblems) {
						importEmblemList_cursorPosition = totalEmblems;
						import_emblemShownFirst = totalEmblems-2;
						if (import_emblemShownFirst < 0) import_emblemShownFirst = 0;
						if (importEmblemList_cursorPositionOnScreen > totalEmblems) {
							importEmblemList_cursorPositionOnScreen = totalEmblems;
						}
					} else if (importEmblemList_cursorPosition > import_emblemShownFirst+2) {
						import_emblemShownFirst++;
					}
					if (importEmblemList_cursorPositionOnScreen > 2) {
						importEmblemList_cursorPositionOnScreen = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				bool exportFound = false;
				if (importPage == 1 && totalEmblems > 0) {
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(importEmblemList_cursorPosition));
							if (access(embFilePath, F_OK) == 0) {
								sndSelect();
								readSS4EmblemFile(cursorPosition, embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(importEmblemList_cursorPosition));
							if (access(embFilePath, F_OK) == 0) {
								sndSelect();
								readSS3EmblemFile(embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(emblemImported, "Imported %s successfully.", getExportedEmblemName(importEmblemList_cursorPosition));
						messageNo = 1;
						subScreenMode = 1;
					} else {
						sndBack();
						messageNo = 3;
					}
					showMessage = true;
				} else if (importPage == 0) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[importEmblemList_cursorPosition]);
							if (access(embFilePath, F_OK) == 0) {
								readSS4EmblemFile(cursorPosition, embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[importEmblemList_cursorPosition]);
							if (access(embFilePath, F_OK) == 0) {
								readSS3EmblemFile(embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(emblemImported, "Imported %s successfully.", import_emblemNames[importEmblemList_cursorPosition]);
						messageNo = 1;
						subScreenMode = 1;
					} else {
						sndBack();
						messageNo = 3;
					}
					showMessage = true;
				}
			}
			if (hDown & KEY_LEFT) {
				sndHighlight();
				importPage--;
				if (importPage < 0) importPage = 1;
				importEmblemList_cursorPosition = 0;
				importEmblemList_cursorPositionOnScreen = 0;
				import_emblemShownFirst = 0;
			}
			if (hDown & KEY_RIGHT) {
				sndHighlight();
				importPage++;
				if (importPage > 1) importPage = 0;
				importEmblemList_cursorPosition = 0;
				importEmblemList_cursorPositionOnScreen = 0;
				import_emblemShownFirst = 0;
			}
			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				subScreenMode = 1;
			}
		} else if (subScreenMode == 1) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					emblemChangeMenu_cursorPosition--;
					if (emblemChangeMenu_cursorPosition < 0) {
						emblemChangeMenu_cursorPosition = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					emblemChangeMenu_cursorPosition++;
					if (emblemChangeMenu_cursorPosition > 1) {
						emblemChangeMenu_cursorPosition = 1;
					}
				}
			}
			if (hDown & KEY_A) {
				if (emblemChangeMenu_cursorPosition == 1) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem %i.chr", cursorPosition);
							writeSS4EmblemFile(cursorPosition, embFilePath);
							break;
						case 2:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem.chr");
							writeSS3EmblemFile(embFilePath);
							break;
					}
					messageNo = 2;
					showMessage = true;
				} else {
					sndSelect();
					subScreenMode = 2;
					displayNothing = true;
					gspWaitForVBlank();
					getExportedEmblemContents();
					displayNothing = false;
				}
			}
			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				subScreenMode = 0;
			}
		} else {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					cursorPosition--;
					if (cursorPosition < 0) {
						cursorPosition = 0;
					}
					modeInited = false;
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					cursorPosition++;
					if (cursorPosition > totalEmblems) {
						cursorPosition = totalEmblems;
					}
					modeInited = false;
				}
			}
			if (hDown & KEY_A) {
				sndSelect();
				subScreenMode = 1;
			}
			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
				fadeout = true;
			}
		}
	}
}
