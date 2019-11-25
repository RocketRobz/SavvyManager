#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"

#include "ss4charnames.h"
#include "import_ss4charnames.h"

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

static int subScreenMode = 0;
/*
	0: Character list
	1: What to change
	2: Body change
	3: Outfit change (Currently bow placement change)
	4: Import character
*/

static int import_highlightedGame = 0;

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

static u16 totalCharacters = 0;
static u16 import_totalCharacters = 0;

static int characterList_cursorPosition = 0;
static int characterList_cursorPositionOnScreen = 0;

static int characterChangeMenu_cursorPosition = 0;

static int importCharacterList_cursorPosition = 0;
static int importCharacterList_cursorPositionOnScreen = 0;

static int characterShownFirst = 0;
static int import_characterShownFirst = 0;

void changeCharacter(void) {
	if (highlightedGame == 3) {
		totalCharacters = 0x20;
		readSS4Save();
	}

	if (import_highlightedGame == 3) {
		import_totalCharacters = 0xC;
	}

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

	if (subScreenMode == 4) {
		cursorX = 256;
		cursorY = 64+(48*importCharacterList_cursorPositionOnScreen);

		switch (import_highlightedGame) {
			case 4:
				Draw_Text(32, 8, 0.50, BLACK, "Savvy Manager");
				break;
			case 3:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Styling Star");
				break;
			case 2:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Fashion Forward");
				break;
			case 1:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Trendsetters");
				break;
			case 0:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy");
				break;
		}
		Draw_Text(8, 8, 0.50, BLACK, "<");
		Draw_Text(304, 8, 0.50, BLACK, ">");

		int i2 = 48;
		for (int i = import_characterShownFirst; i < import_characterShownFirst+3; i++) {
			if (import_highlightedGame == 3) {
				Gui::sprite((import_ss4CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, import_ss4CharacterNames[i]);
			}
			i2 += 48;
		}
	} else if (subScreenMode == 1) {
		cursorX = 256;
		cursorY = 64+(48*characterChangeMenu_cursorPosition);

		Draw_Text(8, 8, 0.50, BLACK, (characterList_cursorPosition==0 ? ss4PlayerName : ss4CharacterNames[characterList_cursorPosition]));

		int i2 = 48;
		Draw_Text(64, i2, 0.65, BLACK, "Change body");
		if (highlightedGame == 3) {
			i2 += 48;
			Draw_Text(64, i2, 0.65, BLACK, "Change bow placement");
		}
		i2 += 48;
		Draw_Text(64, i2, 0.65, BLACK, "Inport character");
	} else {
		cursorX = 256;
		cursorY = 64+(48*characterList_cursorPositionOnScreen);

		Draw_Text(8, 8, 0.50, BLACK, "Select the character you want to change.");

		int i2 = 48;
		if (highlightedGame == 3) {
			for (int i = characterShownFirst; i < characterShownFirst+3; i++) {
				if (i==0) {
					Gui::sprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss4PlayerName);
				} else {
					Gui::sprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss4CharacterNames[i]);
				}
				i2 += 48;
			}
		}
	}

	Gui::sprite(sprites_button_shadow_idx, 5, 199);
	Gui::sprite(sprites_button_red_idx, 5, 195);
	Gui::sprite(sprites_arrow_back_idx, 19, 195);
	Gui::sprite(sprites_button_b_idx, 44, 218);

	drawCursor();

	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Draw_EndFrame();

	if(!fadein) {
		if (subScreenMode == 4) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					importCharacterList_cursorPosition--;
					importCharacterList_cursorPositionOnScreen--;
					if (importCharacterList_cursorPosition < 0) {
						importCharacterList_cursorPosition = 0;
						import_characterShownFirst = 0;
					} else if (importCharacterList_cursorPosition < import_characterShownFirst) {
						import_characterShownFirst--;
					}
					if (importCharacterList_cursorPositionOnScreen < 0) {
						importCharacterList_cursorPositionOnScreen = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					importCharacterList_cursorPosition++;
					importCharacterList_cursorPositionOnScreen++;
					if (importCharacterList_cursorPosition > import_totalCharacters) {
						importCharacterList_cursorPosition = import_totalCharacters;
						import_characterShownFirst = import_totalCharacters-2;
					} else if (importCharacterList_cursorPosition > import_characterShownFirst+2) {
						import_characterShownFirst++;
					}
					if (importCharacterList_cursorPositionOnScreen > 2) {
						importCharacterList_cursorPositionOnScreen = 2;
					}
				}
			}
			/*if (hDown & KEY_A) {
				subScreenMode = 1;
			}*/
			if ((hDown & KEY_L)
			|| (hDown & KEY_LEFT)) {
				import_highlightedGame--;
				if (import_highlightedGame < 0) import_highlightedGame = 4;
			}
			if ((hDown & KEY_R)
			|| (hDown & KEY_RIGHT)) {
				import_highlightedGame++;
				if (import_highlightedGame > 4) import_highlightedGame = 0;
			}
			if (hDown & KEY_B) {
				subScreenMode = 1;
			}
		} else if (subScreenMode == 1) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					characterChangeMenu_cursorPosition--;
					if (characterChangeMenu_cursorPosition < 0) {
						characterChangeMenu_cursorPosition = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					characterChangeMenu_cursorPosition++;
					if (characterChangeMenu_cursorPosition > 2) {
						characterChangeMenu_cursorPosition = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				subScreenMode = 4;
			}
			if (hDown & KEY_B) {
				subScreenMode = 0;
			}
		} else {
			if (showCursor) {
				if (hDown & KEY_UP) {
					characterList_cursorPosition--;
					characterList_cursorPositionOnScreen--;
					if (characterList_cursorPosition < 0) {
						characterList_cursorPosition = 0;
						characterShownFirst = 0;
					} else if (characterList_cursorPosition < characterShownFirst) {
						characterShownFirst--;
					}
					if (characterList_cursorPositionOnScreen < 0) {
						characterList_cursorPositionOnScreen = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					characterList_cursorPosition++;
					characterList_cursorPositionOnScreen++;
					if (characterList_cursorPosition > totalCharacters) {
						characterList_cursorPosition = totalCharacters;
						characterShownFirst = totalCharacters-2;
					} else if (characterList_cursorPosition > characterShownFirst+2) {
						characterShownFirst++;
					}
					if (characterList_cursorPositionOnScreen > 2) {
						characterList_cursorPositionOnScreen = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				subScreenMode = 1;
			}
			if (hDown & KEY_B) {
				screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
				fadeout = true;
			}
		}
	}
}
