#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "common.hpp"
#include "dumpdsp.h"
#include "savedata.h"
//#include "settings.h"
#include "sound.h"
#include "thread.h"

Handle threadRequest;

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

static char verText[32];

extern void loadSettings(void);

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_EXIT = -1,				// Exit homebrew
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_WHAT_TO_DO = 2,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 3,	// Change character
	SCREEN_MODE_CHANGE_MUSIC = 4,		// Change music
	SCREEN_MODE_CHANGE_EMBLEM = 5,		// Change emblem
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

u8 sysRegion = CFG_REGION_USA;
int highlightedGame = 1;

int fadealpha = 255;
int fadecolor = 0;
bool fadein = true;
bool fadeout = false;
	
int text_width = 0;
const char* yeartext = "2019-2020 RocketRobz";
//const char* yeartext2 = "Games 2008-2017 Nintendo & syn Sophia";
	
float bg_xPos = 0.0f;
float bg_yPos = 0.0f;

bool showCursor = false;
int cursorX = 0;
int cursorY = 0;
static int whatToChange_cursorPosition = 0;
int cursorAlpha = 0;

static bool showMessage = false;
static int messageNo = 0;

static void drawCannotEditMsg(void) {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 2, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -2, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	if (messageNo == 1) {
		if (highlightedGame == 0) {
			Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Save data not found.");
			Gui::DrawStringCentered(0, 90, 0.60, BLACK, /*highlightedGame==3 ? "Please back up the extra data using" :*/ "Please back it up using");
			Gui::DrawStringCentered(0, 110, 0.60, BLACK, "Checkpoint, and name the backup:");
			Gui::DrawStringCentered(0, 134, 0.60, BLACK, "SavvyManager");
		} else {
			Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Save data not found.");
		}
	} else {
		switch (sysRegion) {
			default:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Style Savvy's");
				break;
			case CFG_REGION_EUR:
			case CFG_REGION_AUS:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Style Boutique's");
				break;
			case CFG_REGION_JPN:
				Gui::DrawStringCentered(0, 92, 0.60, BLACK, "Cannot edit Girls Mode's");
				break;
		}
		Gui::DrawStringCentered(0, 112, 0.60, BLACK, "save data yet.");
	}
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

u32 hDown = 0;
touchPosition touch;

bool touchingBackButton(void) {
	return (touch.px >= 7 && touch.px < 7+40 && touch.py >= 197 && touch.py < 197+44);
}

static bool runSelection = false;

static bool ss2SaveFound = false;
static bool ss3SaveFound = false;
static bool ss4SaveFound = false;

static bool runThreads = true;

void controlThread(void) {
	while (runThreads) {
		svcWaitSynchronization(threadRequest, U64_MAX);

		if (screenmode == SCREEN_MODE_GAME_SELECT) {
			//Play_Music();

			if (showMessage) {
				if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
					sndSelect();
					showMessage = false;
				}
			} else if (!fadein && !fadeout) {
				if ((hDown & KEY_LEFT) || ((hDown & KEY_TOUCH) && touch.px >= 0 && touch.px < 32 && touch.py >= 104 && touch.py < 104+32)) {
					sndHighlight();
					highlightedGame--;
					if (highlightedGame < 0) highlightedGame = 3;
				} else if ((hDown & KEY_RIGHT) || ((hDown & KEY_TOUCH) && touch.px >= 320-32 && touch.px < 320 && touch.py >= 104 && touch.py < 104+32)) {
					sndHighlight();
					highlightedGame++;
					if (highlightedGame > 3) highlightedGame = 0;
				}

				if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 32 && touch.px < 320-32 && touch.py >= 56 && touch.py < 56+128)) {
				  if (highlightedGame==0) {
					sndBack();
					messageNo = 0;
					showMessage = true;
				  } else if ((highlightedGame==1 && ss2SaveFound)
				  || (highlightedGame==2 && ss3SaveFound)
				  || (highlightedGame==3 && ss4SaveFound))
				  {
					sndSelect();
					screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
					fadeout = true;
				  } else {
					sndBack();
					messageNo = 1;
					showMessage = true;
				  }
				}

				if (hDown & KEY_START) {
					sndBack();
					screenmodebuffer = SCREEN_MODE_EXIT;
					fadecolor = 0;
					fadeout = true;
				}
			}
		} else if (screenmode == SCREEN_MODE_WHAT_TO_DO) {
			if (showMessage) {
				if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
					sndSelect();
					showMessage = false;
				}
			} else if (!fadein && !fadeout) {
				if (highlightedGame > 0 && showCursor) {
					if (hDown & KEY_LEFT) {
						sndHighlight();
						if (highlightedGame > 1) {
							if (whatToChange_cursorPosition == 2) whatToChange_cursorPosition = 0;
							else if (whatToChange_cursorPosition == 0) whatToChange_cursorPosition = 2;
						} else {
							whatToChange_cursorPosition--;
							if (whatToChange_cursorPosition < 0) whatToChange_cursorPosition = 1;
						}
					} else if (hDown & KEY_RIGHT) {
						sndHighlight();
						if (highlightedGame > 1) {
							if (whatToChange_cursorPosition == 0) whatToChange_cursorPosition = 2;
							else if (whatToChange_cursorPosition == 2) whatToChange_cursorPosition = 0;
						} else {
							whatToChange_cursorPosition++;
							if (whatToChange_cursorPosition > 1) whatToChange_cursorPosition = 0;
						}
					}
				}
				if (hDown & KEY_A) {
					runSelection = true;
				}
				if ((hDown & KEY_TOUCH) && touch.px >= 71 && touch.px <= 248 && touch.py >= 91 && touch.py <= 136) {
					if (touch.px < 120) {
						whatToChange_cursorPosition = 0;
						runSelection = true;
					}
					if ((touch.px > 134) && (touch.px < 185) && highlightedGame==1) {
						whatToChange_cursorPosition = 1;
						runSelection = true;
					}
					if ((touch.px > 198) && highlightedGame > 1) {
						whatToChange_cursorPosition = 2;
						runSelection = true;
					}
				}
				if (runSelection) {
					sndSelect();
					switch (whatToChange_cursorPosition) {
						case 0:
							if ((highlightedGame==1 && ss2SaveFound)
							  || (highlightedGame==2 && ss3SaveFound)
							  || (highlightedGame==3 && ss4SaveFound))
							  {
								screenmodebuffer = SCREEN_MODE_CHANGE_CHARACTER;
							  } else {
							  	sndBack();
								messageNo = 1;
								showMessage = true;
							  }
							break;
						case 1:
							screenmodebuffer = SCREEN_MODE_CHANGE_MUSIC;
							break;
						case 2:
							screenmodebuffer = SCREEN_MODE_CHANGE_EMBLEM;
							break;
					}
					fadeout = true;
					runSelection = false;
				}
				if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
					sndBack();
					screenmodebuffer = SCREEN_MODE_GAME_SELECT;
					fadeout = true;
				}
			}
		} else if (screenmode == SCREEN_MODE_CHANGE_CHARACTER) {
			extern void changeCharacter(void);
			changeCharacter();
		} else if (screenmode == SCREEN_MODE_CHANGE_MUSIC) {
			extern void changeMusic(void);
			changeMusic();
		} else if (screenmode == SCREEN_MODE_CHANGE_EMBLEM) {
			extern void changeEmblem(void);
			changeEmblem();
		}
		svcClearEvent(threadRequest);
	}
}

int main()
{
	screenoff();

	aptInit();
	amInit();
	romfsInit();
	srvInit();
	hidInit();
	Result res = cfguInit();
	if (R_SUCCEEDED(res)) {
		CFGU_SecureInfoGetRegion(&sysRegion);
		cfguExit();
	}

	gfxInitDefault();
	
	Gui::init();
	GFX::loadSheets();

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);
	mkdir("sdmc:/3ds/SavvyManager", 0777);
	mkdir("sdmc:/3ds/SavvyManager/emblems", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS2", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS2/characters", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS2/musicPacks", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS3", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS3/characters", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS4", 0777);
	mkdir("sdmc:/3ds/SavvyManager/SS4/characters", 0777);

	mkdir("sdmc:/luma", 0777);
	mkdir("sdmc:/luma/titles", 0777);

	switch (sysRegion) {
		default:
			// Style Savvy: Trendsetters folders
			mkdir("sdmc:/luma/titles/00040000000A9100", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9100/romfs", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9100/romfs/Common", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9100/romfs/Common/Sound", 0777);
			break;
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			// New Style Boutique folders
			mkdir("sdmc:/luma/titles/00040000000A9000", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9000/romfs", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9000/romfs/Common", 0777);
			mkdir("sdmc:/luma/titles/00040000000A9000/romfs/Common/Sound", 0777);
			break;
		case CFG_REGION_JPN:
			// Wagamama Fashion: Girls Mode - Yokubari Sengen folders
			mkdir("sdmc:/luma/titles/000400000005D100", 0777);
			mkdir("sdmc:/luma/titles/000400000005D100/romfs", 0777);
			mkdir("sdmc:/luma/titles/000400000005D100/romfs/Common", 0777);
			mkdir("sdmc:/luma/titles/000400000005D100/romfs/Common/Sound", 0777);
			break;
	}
	//mkdir("sdmc:/luma/titles/00040000000A9100/romfs/Common/Sound/stream", 0777);

 	// Style Savvy: Fashion Forward folders
	//mkdir("sdmc:/luma/titles/0004000000196500", 0777);

	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	}else{
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::ScreenDraw(Bottom);
		Gui::DrawString(12, 16, 0.5f, WHITE, "Dumping DSP firm...");
		C3D_FrameEnd(0);
		screenon();
		dumpDsp();
		if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
			ndspInit();
			dspfirmfound = true;
		} else {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Bottom, TRANSPARENT); // clear Bottom Screen to avoid Text overdraw.
			Gui::ScreenDraw(Bottom);
			Gui::DrawString(12, 16, 0.5f, WHITE, "DSP firm dumping failed.\n"
					"Running without sound.");
			C3D_FrameEnd(0);
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

	u32 ss2Id = 0x000A9100;
	u32 ss3Id = 0x00196500;

	int ss1Logo = gameSelSprites_title1_idx;
	int ss2Screenshot = gameSelSprites_title2_screenshot_idx;
	int ss2Logo = gameSelSprites_title2_idx;
	int ssLogoXpos = 0;
	int ss3Logo = gameSelSprites_title3_idx;
	int ss4Logo = gameSelSprites_title4_idx;

	switch (sysRegion) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss2Id = 0x000A9000;
			ss3Id = 0x0016A100;
			ss2Screenshot = gameSelSprites_title2_screenshotJ_idx;
			ss1Logo = gameSelSprites_title1_E_idx;
			ss2Logo = gameSelSprites_title2_E_idx;
			ss3Logo = gameSelSprites_title3_E_idx;
			ss4Logo = gameSelSprites_title4_E_idx;
			ssLogoXpos = 32;
			break;
		case CFG_REGION_JPN:
			ss2Id = 0x0005D100;
			//ss3Id = 0x0016A100;
			ss2Screenshot = gameSelSprites_title2_screenshotJ_idx;
			break;
		default:
			break;
	}

	const u32 path2[3] = {MEDIATYPE_SD, ss2Id, 0x00040000};
	const u32 path2card[3] = {MEDIATYPE_GAME_CARD, ss2Id, 0x00040000};
	const u32 path3[3] = {MEDIATYPE_SD, ss3Id, 0x00040000};
	const u32 path3card[3] = {MEDIATYPE_GAME_CARD, ss3Id, 0x00040000};
	const u32 path4[3] = {MEDIATYPE_SD, 0x00001C25, 0};

	res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path2}, "ss2");	// Read from digital version
	if (R_FAILED(res)) {
		res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path2card}, "ss2");	// Read from game card
	}

	res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path3}, "ss3");	// Read from digital version
	if (R_FAILED(res)) {
		res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path3card}, "ss3");	// Read from game card
	}

	//archiveMount(ARCHIVE_EXTDATA, {PATH_BINARY, 12, path4}, "ss4");
	FSUSER_OpenArchive(&archive4, ARCHIVE_EXTDATA, {PATH_BINARY, 12, path4});
	//FSUSER_OpenDirectory(&handle4, archive4, fsMakePath(PATH_UTF16, "/"));

	ss2SaveFound = (access(ss2SavePath, F_OK) == 0);
	ss3SaveFound = (access(ss3SavePath, F_OK) == 0);
	ss4SaveFound = foundSS4Save();
	
	sprintf(verText, "Ver. %i.%i.%i", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	loadSettings();

	screenon();

	svcCreateEvent(&threadRequest,(ResetType)0);
	createThread((ThreadFunc)controlThread);

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		// Scan hid shared memory for input events
		hidScanInput();

		hDown = hidKeysDown();
		//const u32 hHeld = hidKeysHeld();

		hidTouchRead(&touch);

		if (screenmode != SCREEN_MODE_ROCKETROBZ) {
			screenDelay = 0;
		}

		if (screenmode == SCREEN_MODE_EXIT) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, TRANSPARENT);
			C2D_TargetClear(Bottom, TRANSPARENT);
			Gui::clearTextBufs();
			Gui::ScreenDraw(Top);
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, 255)); // Fade in/out effect
			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, 255)); // Fade in/out effect
			C3D_FrameEnd(0);
			break;
		} else if (screenmode == SCREEN_MODE_ROCKETROBZ) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, TRANSPARENT);
			C2D_TargetClear(Bottom, TRANSPARENT);
			Gui::clearTextBufs();
			Gui::ScreenDraw(Top);

			GFX::DrawSprite(sprites_logo_rocketrobz_idx, 0, 0);
			Gui::DrawString(8, 220, 0.50, BLACK, yeartext);
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 240, WHITE);
			/*text_width = 104;
			Gui::DrawString(((320-text_width)/2), 100, 0.50, BLACK, yeartext);
			text_width = 264;
			Gui::DrawString(((320-text_width)/2), 116, 0.50, BLACK, yeartext2);*/
			//GFX::DrawSprite(sprites_logo_SSanniversary_idx, 32, 24);
			GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			C3D_FrameEnd(0);

			screenDelay++;
			if(screenDelay > 60*3){
				screenmodebuffer = SCREEN_MODE_GAME_SELECT;
				fadeout = true;
			}
		} else if (screenmode == SCREEN_MODE_GAME_SELECT) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, TRANSPARENT);
			C2D_TargetClear(Bottom, TRANSPARENT);
			Gui::clearTextBufs();
			Gui::ScreenDraw(Top);

			/*for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					Gui::sprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
				}
			}*/
			switch(highlightedGame) {
				case 0:
				default:
					GFX::DrawGameSelSprite(gameSelSprites_title1_screenshot_idx, 0, 0);
					break;
				case 1:
					GFX::DrawGameSelSprite(ss2Screenshot, 0, 0);
					break;
				case 2:
					GFX::DrawGameSelSprite(gameSelSprites_title3_screenshot_idx, 0, 0);
					break;
				case 3:
					GFX::DrawGameSelSprite(gameSelSprites_title4_screenshot_idx, 0, 0);
					break;
			}
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			Gui::DrawString(8, 8, 0.50, BLACK, "Select a game to manage it's save data.");
			switch(highlightedGame) {
				case 0:
				default:
					GFX::DrawGameSelSprite(ss1Logo, ssLogoXpos, 56);
					break;
				case 1:
					GFX::DrawGameSelSprite(ss2Logo, ssLogoXpos, 56);
					break;
				case 2:
					GFX::DrawGameSelSprite(ss3Logo, 0, 56);
					break;
				case 3:
					GFX::DrawGameSelSprite(ss4Logo, ssLogoXpos, 56);
					break;
			}
			Gui::DrawString(8, 112, 0.55, BLACK, "<");
			Gui::DrawString(304, 112, 0.55, BLACK, ">");
			Gui::DrawString(248, 220, 0.50, BLACK, verText);
			if (showMessage) {
				drawCannotEditMsg();
			}
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			C3D_FrameEnd(0);
		} else if (screenmode == SCREEN_MODE_WHAT_TO_DO) {
			if ((highlightedGame == 0)
			|| (highlightedGame > 1 && whatToChange_cursorPosition == 1)
			|| (highlightedGame < 2 && whatToChange_cursorPosition == 2)) {
				whatToChange_cursorPosition = 0;
			}

			switch (whatToChange_cursorPosition) {
				case 0:
				default:
					cursorX = 80;
					cursorY = 104;
					break;
				case 1:
					cursorX = 148;
					cursorY = 104;
					break;
				case 2:
					cursorX = 212;
					cursorY = 104;
					break;
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
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
			for(int w = 0; w < 7; w++) {
				for(int h = 0; h < 3; h++) {
					GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
				}
			}
			Gui::DrawString(8, 8, 0.50, BLACK, "What do you want to change?");
			int iconXpos = 64;
			GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
			GFX::DrawSprite(sprites_icon_profile_idx, iconXpos, 80);
			Gui::DrawString(iconXpos-2, 140, 0.50, RED, "Characters");
			iconXpos += 64;
			if (highlightedGame == 1) {
				// Show music pack option for Trendsetters
				GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
				GFX::DrawSprite(sprites_icon_music_idx, iconXpos, 80);
				Gui::DrawString(iconXpos+14, 140, 0.50, RED, "Music");
			}
			iconXpos += 64;
			if (highlightedGame > 1) {
				// Show emblem option for Fashion Forward and Styling Star
				GFX::DrawSpriteBlend(sprites_icon_shadow_idx, iconXpos, 86, C2D_Color32(0, 0, 0, 63));
				GFX::DrawSprite(sprites_icon_emblem_idx, iconXpos, 80);
				Gui::DrawString(iconXpos+8, 140, 0.50, RED, "Emblem");
			}
			GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
			GFX::DrawSprite(sprites_button_red_idx, 5, 195);
			GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
			GFX::DrawSprite(sprites_button_b_idx, 44, 218);
			/*GFX::DrawSprite(sprites_button_shadow_idx, 251, 199);
			GFX::DrawSprite(sprites_button_blue_idx, 251, 195);*/
			GFX::drawCursor();
			if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
			C3D_FrameEnd(0);
		} else if (screenmode == SCREEN_MODE_CHANGE_CHARACTER) {
			extern void changeCharacterGraphics(void);
			changeCharacterGraphics();
		} else if (screenmode == SCREEN_MODE_CHANGE_MUSIC) {
			extern void changeMusicGraphics(void);
			changeMusicGraphics();
		} else if (screenmode == SCREEN_MODE_CHANGE_EMBLEM) {
			extern void changeEmblemGraphics(void);
			changeEmblemGraphics();
		}
		// Scroll background
		bg_xPos += 0.3;
		if(bg_xPos >= 72) bg_xPos = 0.0f;
		bg_yPos -= 0.3;
		if(bg_yPos <= -136) bg_yPos = 0.0f;

		if (hDown) {
			svcSignalEvent(threadRequest);
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
				if (screenmode != SCREEN_MODE_EXIT) {
					fadein = true;
				}
				fadeout = false;
			}
		}
	}


	runThreads = false;

	commitSaveData();
	archiveUnmountAll();

	//delete music;
	delete sfx_select;
	delete sfx_back;
	delete sfx_highlight;
	if (dspfirmfound) {
		ndspExit();
	}

	// signal the thread and wait for it to exit
	svcSignalEvent(threadRequest);
	destroyThreads();

	// close event handle
	svcCloseHandle(threadRequest);

	Gui::exit();
	GFX::unloadSheets();

	gfxExit();
	hidExit();
	srvExit();
	romfsExit();
	aptExit();
	amExit();

	return 0;
}