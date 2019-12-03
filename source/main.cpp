#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "dumpdsp.h"
#include "gui.hpp"
#include "savedata.h"
#include "sound.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_WHAT_TO_DO = 2,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 3,	// Change character
};
static int screenmode = 0;
int screenmodebuffer = 0;

static int screenDelay = 0;

//sound *music = NULL;
sound *sfx_select = NULL;
sound *sfx_back = NULL;
sound *sfx_highlight = NULL;

bool dspfirmfound = false;
//static bool musicPlaying = false;
static bool screenoff_ran = false;
static bool screenon_ran = true;

/*static void Play_Music(void) {
	if (!musicPlaying && dspfirmfound) {
		music->play();
		musicPlaying = true;
	}
}*/

void sndSelect(void) {
	if (dspfirmfound) {
		sfx_select->stop();
		sfx_select->play();
	}
}

void sndBack(void) {
	if (dspfirmfound) {
		sfx_back->stop();
		sfx_back->play();
	}
}

void sndHighlight(void) {
	if (dspfirmfound) {
		sfx_highlight->stop();
		sfx_highlight->play();
	}
}

void screenoff(void)
{
	screenon_ran = false;
	if(!screenoff_ran) {
		if (R_SUCCEEDED(gspLcdInit())) {
			GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);
			gspLcdExit();
		}
		screenoff_ran = true;
	}
}

void screenon(void)
{
 	screenoff_ran = false;
	if(!screenon_ran) {
		if (R_SUCCEEDED(gspLcdInit())) {
			GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);
			gspLcdExit();
		}
		screenon_ran = true;
	}
}

int highlightedGame = 0;

int fadealpha = 255;
int fadecolor = 0;
bool fadein = true;
bool fadeout = false;
	
int text_width = 0;
const char* yeartext = "2019 RocketRobz";
//const char* yeartext2 = "Games 2008-2017 Nintendo & syn Sophia";
	
float bg_xPos = 0.0f;
float bg_yPos = 0.0f;

bool showCursor = false;
int cursorX = 0;
int cursorY = 0;
int whatToChange_cursorPosition = 0;
int cursorAlpha = 0;

void drawCursor(void) {
	if (cursorAlpha == 255) {
		Gui::sprite(sprites_cursor_idx, cursorX, cursorY);
	} else if (cursorAlpha > 0) {
		Gui::Draw_ImageBlend(sprites_cursor_idx, cursorX, cursorY, C2D_Color32(255, 255, 255, cursorAlpha));
	}
}

u32 hDown = 0;

static bool ss2SaveFound = false;
static bool ss3SaveFound = false;
static bool ss4SaveFound = false;

int main()
{
	screenoff();

	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();

	gfxInitDefault();
	
	Gui::init();

	// make folders if they don't exist
	//mkdir("sdmc:/3ds", 0777);
	//mkdir("sdmc:/3ds/SavvyGameSelect", 0777);

 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	}else{
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(bottom);
		Draw_Text(12, 16, 0.5f, WHITE, "Dumping DSP firm...");
		Draw_EndFrame();
		screenon();
		dumpDsp();
		if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
			ndspInit();
			dspfirmfound = true;
		} else {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			set_screen(bottom);
			Draw_Text(12, 16, 0.5f, WHITE, "DSP firm dumping failed.\n"
					"Running without sound.");
			Draw_EndFrame();
			for (int i = 0; i < 90; i++) {
				gspWaitForVBlank();
			}
		}
	}

	// Load the sound effects if DSP is available.
	if (dspfirmfound) {
		//music = new sound("romfs:/sounds/music.wav", 1, true);
		sfx_select = new sound("romfs:/sounds/select.wav", 2, false);
		sfx_back = new sound("romfs:/sounds/back.wav", 3, false);
		sfx_highlight = new sound("romfs:/sounds/highlight.wav", 4, false);
	}
	
	ss2SaveFound = (access(ss2SavePath, F_OK) == 0);
	ss3SaveFound = (access(ss3SavePath, F_OK) == 0);
	ss4SaveFound = (access(ss4SavePath, F_OK) == 0);

	screenon();

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		// Scan hid shared memory for input events
		hidScanInput();

		hDown = hidKeysDown();
		//const u32 hHeld = hidKeysHeld();

		if(screenmode != SCREEN_MODE_ROCKETROBZ) {
			screenDelay = 0;
		}

		if(screenmode == SCREEN_MODE_ROCKETROBZ) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, TRANSPARENT);
			C2D_TargetClear(bottom, TRANSPARENT);
			Gui::clearTextBufs();
			set_screen(top);

			Gui::sprite(sprites_logo_rocketrobz_idx, 0, 0);
			Draw_Text(8, 220, 0.50, BLACK, yeartext);
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			set_screen(bottom);
			Draw_Rect(0, 0, 320, 240, WHITE);
			/*text_width = 104;
			Draw_Text(((320-text_width)/2), 100, 0.50, BLACK, yeartext);
			text_width = 264;
			Draw_Text(((320-text_width)/2), 116, 0.50, BLACK, yeartext2);*/
			Gui::sprite(sprites_logo_SSanniversary_idx, 32, 24);
			if (fadealpha > 0) Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			screenDelay++;
			if(screenDelay > 60*3){
				screenmodebuffer = SCREEN_MODE_GAME_SELECT;
				fadeout = true;
			}
		} else if(screenmode == SCREEN_MODE_GAME_SELECT) {
			//Play_Music();

			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, TRANSPARENT);
			C2D_TargetClear(bottom, TRANSPARENT);
			Gui::clearTextBufs();
			set_screen(top);

			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			switch(highlightedGame) {
				case 0:
				default:
					Gui::sprite(sprites_title1_screenshot_idx, 0, 0);
					break;
				case 1:
					Gui::sprite(sprites_title2_screenshot_idx, 0, 0);
					break;
				case 2:
					Gui::sprite(sprites_title3_screenshot_idx, 0, 0);
					break;
				case 3:
					Gui::sprite(sprites_title4_screenshot_idx, 0, 0);
					break;
			}
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			set_screen(bottom);
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			Draw_Text(8, 8, 0.50, BLACK, "Select a game to manage it's save data.");
			switch(highlightedGame) {
				case 0:
				default:
					Gui::sprite(sprites_title1_idx, 0, 56);
					break;
				case 1:
					Gui::sprite(sprites_title2_idx, 0, 56);
					break;
				case 2:
					Gui::sprite(sprites_title3_idx, 0, 56);
					break;
				case 3:
					Gui::sprite(sprites_title4_idx, 0, 56);
					break;
			}
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			if (!fadein) {
				if (hDown & KEY_LEFT) {
					sndHighlight();
					highlightedGame--;
					if (highlightedGame < 0) highlightedGame = 3;
				} else if (hDown & KEY_RIGHT) {
					sndHighlight();
					highlightedGame++;
					if (highlightedGame > 3) highlightedGame = 0;
				}

				if (hDown & KEY_A) {
				  if (highlightedGame==0
				  || (highlightedGame==1 && ss2SaveFound)
				  || (highlightedGame==2 && ss3SaveFound)
				  || (highlightedGame==3 && ss4SaveFound))
				  {
					sndSelect();
					screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
					fadeout = true;
				  } else {
					sndBack();
				  }
				}
			}
		} else if(screenmode == SCREEN_MODE_WHAT_TO_DO) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, TRANSPARENT);
			C2D_TargetClear(bottom, TRANSPARENT);
			Gui::clearTextBufs();
			set_screen(top);

			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			set_screen(bottom);
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			Draw_Text(8, 8, 0.50, BLACK, "What do you want to change?");
			Gui::Draw_ImageBlend(sprites_icon_shadow_idx, 64, 86, C2D_Color32(0, 0, 0, 63));
			Gui::sprite(sprites_icon_profile_idx, 64, 80);
			Draw_Text(62, 140, 0.50, RED, "Characters");
			Gui::sprite(sprites_button_shadow_idx, 5, 199);
			Gui::sprite(sprites_button_red_idx, 5, 195);
			Gui::sprite(sprites_arrow_back_idx, 19, 195);
			Gui::sprite(sprites_button_b_idx, 44, 218);
			/*Gui::sprite(sprites_button_shadow_idx, 251, 199);
			Gui::sprite(sprites_button_blue_idx, 251, 195);*/
			drawCursor();
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			if (!fadein) {
				if (hDown & KEY_A) {
					sndSelect();
					screenmodebuffer = SCREEN_MODE_CHANGE_CHARACTER;
					fadeout = true;
				}
				if (hDown & KEY_B) {
					sndBack();
					screenmodebuffer = SCREEN_MODE_GAME_SELECT;
					fadeout = true;
				}
			}

			switch (whatToChange_cursorPosition) {
				case 0:
				default:
					cursorX = 80;
					cursorY = 104;
					break;
			}
		} else if(screenmode == SCREEN_MODE_CHANGE_CHARACTER) {
			extern void changeCharacter(void);
			changeCharacter();
		}

		if ((hDown & KEY_UP)
		|| (hDown & KEY_DOWN)
		|| (hDown & KEY_LEFT)
		|| (hDown & KEY_RIGHT))
		{
			showCursor = true;
		} else if (hDown & KEY_TOUCH)
		{
			showCursor = false;
		}

		if (showCursor) {
			cursorAlpha += 32;
			if (cursorAlpha > 255) {
				cursorAlpha = 255;
			}
		} else {
			cursorAlpha -= 32;
			if (cursorAlpha < 0) {
				cursorAlpha = 0;
			}
		}

		bg_xPos += 0.3;
		if(bg_xPos >= 72) bg_xPos = 0.0f;
		bg_yPos -= 0.3;
		if(bg_yPos <= -136) bg_yPos = 0.0f;

		if (fadein) {
			fadealpha -= 6;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadecolor = 255;
				fadein = false;
			}
		}

		if (fadeout) {
			fadealpha += 6;
			if (fadealpha > 255) {
				fadealpha = 255;
				screenmode = screenmodebuffer;
				fadein = true;
				fadeout = false;
			}
		}
	}

	
	//delete music;
	delete sfx_select;
	delete sfx_back;
	delete sfx_highlight;
	if (dspfirmfound) {
		ndspExit();
	}

	Gui::exit();

	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}