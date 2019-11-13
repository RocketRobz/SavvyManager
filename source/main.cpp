#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <sys/stat.h>

#include "gui.hpp"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_CHARACTER_SELECT = 2,	// Character select
};
static int screenmode = 0;
static int screenmodebuffer = 0;

static int screenDelay = 0;

static bool screenoff_ran = false;
static bool screenon_ran = true;

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

	/*pp2d_load_texture_png(bgtex, "romfs:/graphics/phone_bg.png");
	pp2d_load_texture_png(titletex, "romfs:/graphics/title.png");
	pp2d_load_texture_png(title1tex, "romfs:/graphics/title1.png");
	pp2d_load_texture_png(title2tex, "romfs:/graphics/title2.png");
	pp2d_load_texture_png(title3tex, "romfs:/graphics/title3.png");
	pp2d_load_texture_png(title4tex, "romfs:/graphics/title4.png");
	pp2d_load_texture_png(title1shot, "romfs:/graphics/title1_screenshot.png");
	pp2d_load_texture_png(title2shot, "romfs:/graphics/title2_screenshot.png");
	pp2d_load_texture_png(title3shot, "romfs:/graphics/title3_screenshot.png");
	pp2d_load_texture_png(title4shot, "romfs:/graphics/title4_screenshot.png");
	pp2d_load_texture_png(photostudiobg, "romfs:/graphics/photostudio/Studio/blue.png");*/
	
	//titleshot = title1shot;
	
	int highlightedGame = 0;

	int fadealpha = 255;
	int fadecolor = 0;
	bool fadein = true;
	bool fadeout = false;
	
	int text_width = 0;
	const char* yeartext = "2019 RocketRobz";
	const char* yeartext2 = "Games 2008-2017 Nintendo & syn Sophia";
	
	float bg_xPos = 0.0f;
	float bg_yPos = 0.0f;

	screenon();

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		// Scan hid shared memory for input events
		hidScanInput();
		
		const u32 hDown = hidKeysDown();
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
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			set_screen(bottom);
			Draw_Rect(0, 0, 320, 240, WHITE);
			text_width = 104;
			Draw_Text(((320-text_width)/2), 100, 0.50, BLACK, yeartext);
			text_width = 264;
			Draw_Text(((320-text_width)/2), 116, 0.50, BLACK, yeartext2);
			if (fadealpha > 0) Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			screenDelay++;
			if(screenDelay > 60*3){
				screenmodebuffer = SCREEN_MODE_GAME_SELECT;
				fadeout = true;
			}
		} else if(screenmode == SCREEN_MODE_GAME_SELECT) {
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
			if(highlightedGame==0) {
				Gui::sprite(sprites_title1_idx, 0, 0);
			} else {
				Gui::Draw_ImageBlend(sprites_title1_idx, 0, 0, GRAY);
			}
			if(highlightedGame==1) {
				Gui::sprite(sprites_title2_idx, 200, 0);
			} else {
				Gui::Draw_ImageBlend(sprites_title2_idx, 200, 0, GRAY);
			}
			if(highlightedGame==2) {
				Gui::sprite(sprites_title3_idx, 0, 152);
			} else {
				Gui::Draw_ImageBlend(sprites_title3_idx, 0, 152, GRAY);
			}
			if(highlightedGame==3) {
				Gui::sprite(sprites_title4_idx, 200, 152);
			} else {
				Gui::Draw_ImageBlend(sprites_title4_idx, 200, 152, GRAY);
			}
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			set_screen(bottom);
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			Draw_Text(8, 8, 0.50, BLACK, "Select a game to manage it's save data.");
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			if(hDown & KEY_UP) {
				if(highlightedGame==2 || highlightedGame==3) highlightedGame -= 2;
			} else if(hDown & KEY_DOWN) {
				if(highlightedGame==0 || highlightedGame==1) highlightedGame += 2;
			} else if(hDown & KEY_LEFT) {
				if(highlightedGame==1 || highlightedGame==3) highlightedGame--;
			} else if(hDown & KEY_RIGHT) {
				if(highlightedGame==0 || highlightedGame==2) highlightedGame++;
			}

			if((hDown & KEY_A) && (!fadein)){
				screenmodebuffer = SCREEN_MODE_CHARACTER_SELECT;
				fadeout = true;
			}
		} else if(screenmode == SCREEN_MODE_CHARACTER_SELECT) {
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
			if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			Draw_EndFrame();

			if(!fadein) {
				if(hDown & KEY_B){
					screenmodebuffer = SCREEN_MODE_GAME_SELECT;
					fadeout = true;
				}
			}
		}

		bg_xPos += 0.2;
		if(bg_xPos >= 72) bg_xPos = 0.0f;
		bg_yPos -= 0.2;
		if(bg_yPos <= -136) bg_yPos = 0.0f;

		if (fadein == true) {
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

	
	Gui::exit();

	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}