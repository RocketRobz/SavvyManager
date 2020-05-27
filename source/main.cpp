﻿#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "common.hpp"
#include "screenMode.h"
#include "dumpdsp.h"
#include "gameSelect.hpp"
#include "inifile.h"
#include "rocketRobz.hpp"
#include "savedata.h"
#include "screen.hpp"
#include "sound.h"
#include "thread.h"

bool isInit = true;
int delay = 0;
Handle threadRequest;

#define settingsIni "sdmc:/3ds/SavvyManager/settings.ini"
#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

char verText[32];
int studioBg = 0;
int iFps = 60;
std::string currentMusicPack = "";

//sound *music = NULL;
sound *sfx_select = NULL;
sound *sfx_back = NULL;
sound *sfx_highlight = NULL;

bool dspfirmfound = false;
bool exiting = false;
//static bool musicPlaying = false;
static bool screenoff_ran = false;
static bool screenon_ran = true;

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

float bg_xPos = 0.0f;
float bg_yPos = 0.0f;
bool showCursor = false;
int cursorAlpha = 0;

int ss1Logo = gameSelSprites_title1_idx;
int ss2Screenshot = gameShotSprites_title2_screenshot_idx;
int ss2Logo = gameSelSprites_title2_idx;
int ss1LogoXpos = 0;
int ssLogoXpos = 0;
int ss3Logo = gameSelSprites_title3_idx;
int ss4Logo = gameSelSprites_title4_idx;

u32 hDown = 0;
u32 hHeld = 0;
touchPosition touch;

bool touchingBackButton(void) {
	return (touch.px >= 7 && touch.px < 7+40 && touch.py >= 197 && touch.py < 197+44);
}

bool ss2SaveFound = false;
bool ss3SaveFound = false;
bool ss4SaveFound = false;

static bool runThreads = true;

void controlThread(void) {
	while (runThreads) {
		svcWaitSynchronization(threadRequest, U64_MAX);
		Gui::ScreenLogic(hDown, hHeld, touch, true); // Call the logic of the current screen here.
		svcClearEvent(threadRequest);
	}
}

int main()
{
	screenoff();
	amInit();
	romfsInit();
	Result res = cfguInit();
	if (R_SUCCEEDED(res)) {
		CFGU_SecureInfoGetRegion(&sysRegion);
		cfguExit();
	}

	gfxInitDefault();
	loadSettings();

	Gui::init();
	GFX::loadSheets();
	fadein = true;
	fadealpha = 255;

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

	if ( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	} else {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::ScreenDraw(Bottom);
		Gui::DrawString(12, 16, 0.5f, WHITE, "Dumping DSP firm...");
		C3D_FrameEnd(0);
		screenon();
		dumpDsp();
		if ( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
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
	u32 ss4Id = 0x00001C25;

	switch (sysRegion) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss2Id = 0x000A9000;
			ss3Id = 0x0016A100;
			ss4Id = 0x00001C26;
			ss2Screenshot = gameShotSprites_title2_screenshotE_idx;
			ss1Logo = gameSelSprites_title1_E_idx;
			ss2Logo = gameSelSprites_title2_E_idx;
			ss3Logo = gameSelSprites_title3_E_idx;
			ss4Logo = gameSelSprites_title4_E_idx;
			ss1LogoXpos = 32;
			ssLogoXpos = 32;
			break;
		case CFG_REGION_JPN:
			ss2Id = 0x0005D100;
			ss3Id = 0x0012D800;
			ss4Id = 0x000019F6;
			ss2Screenshot = gameShotSprites_title2_screenshotJ_idx;
			ss1Logo = gameSelSprites_title1_J_idx;
			ss2Logo = gameSelSprites_title2_J_idx;
			ss3Logo = gameSelSprites_title3_J_idx;
			ss4Logo = gameSelSprites_title4_J_idx;
			break;
		case CFG_REGION_KOR:
			ss2Id = 0x0005D100;
			ss2Screenshot = gameShotSprites_title2_screenshotJ_idx;
			ss1Logo = gameSelSprites_title1_K_idx;
			ss2Logo = gameSelSprites_title2_K_idx;
			ss1LogoXpos = 64;
			break;
		default:
			break;
	}

	const u32 path2[3] = {MEDIATYPE_SD, ss2Id, 0x00040000};
	const u32 path2card[3] = {MEDIATYPE_GAME_CARD, ss2Id, 0x00040000};
	const u32 path3[3] = {MEDIATYPE_SD, ss3Id, 0x00040000};
	const u32 path3card[3] = {MEDIATYPE_GAME_CARD, ss3Id, 0x00040000};
	const u32 path4[3] = {MEDIATYPE_SD, ss4Id, 0};

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

	C3D_FrameRate(iFps);

	screenon();

	Gui::setScreen(std::make_unique<RocketRobz>(), false); // Set screen to RocketRobz's screen.
	svcCreateEvent(&threadRequest,(ResetType)0);
	createThread((ThreadFunc)controlThread);
	//Play_Music();

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		// Scan hid shared memory for input events
		hidScanInput();

		hDown = hidKeysDown();
		hHeld = hidKeysHeld();

		hidTouchRead(&touch);

		// Here we draw the actual screen.
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, TRANSPARENT);
		C2D_TargetClear(Bottom, TRANSPARENT);
		Gui::clearTextBufs();
		Gui::DrawScreen();
		C3D_FrameEnd(0);
		if (exiting) {
			if (!fadeout)	break;
		}

		if (isInit) {
			delay++;
			if (delay > iFps*3) {
				Gui::setScreen(std::make_unique<GameSelect>(), true); // Set after delay to the GameSelect screen.
				isInit = false;
			}
		}

		// Scroll background
		switch (iFps) {
			default:
				bg_xPos += 0.3;
				bg_yPos -= 0.3;
				break;
			case 30:
				bg_xPos += 0.6;
				bg_yPos -= 0.6;
				break;
			case 24:
				bg_xPos += 0.9;
				bg_yPos -= 0.9;
				break;
		}

		if (bg_xPos >= 72) bg_xPos = 0.0f;
		if (bg_yPos <= -136) bg_yPos = 0.0f;

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

		int fadeFPS;
		switch (iFps) {
			default:
				fadeFPS = 6;
				break;
			case 30:
				fadeFPS = 12;
				break;
			case 24:
				fadeFPS = 14;
				break;
		}

		Gui::fadeEffects(fadeFPS, fadeFPS);
	}


	runThreads = false;

	saveSettings();

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
	romfsExit();
	amExit();

	return 0;
}