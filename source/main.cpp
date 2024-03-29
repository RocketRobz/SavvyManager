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
#include "danceCpk.hpp"
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
bool ss3DLCharactersBackedUp = false;

int titleSelection = 0;

sound *music = NULL;
sound *music_loop = NULL;
sound *mus_logos = NULL;
sound *sfx_select = NULL;
sound *sfx_back = NULL;
sound *sfx_highlight = NULL;

bool horiHd = true;
bool dspfirmfound = false;
bool exiting = false;
bool musicPlayStarted = false;
static bool musicPlaying = false;
static bool musicLoopPlaying = false;
static int musicLoopDelay = 0;

int ss1Region = -1;
static int ss2Region = -1;
static int ss3Region = -1;
static int ss4Region = -1;

void loadSettings(void) {
	CIniFile settingsini(settingsIni);

	//studioBg = settingsini.GetInt("SAVVY-MANAGER", "STUDIO_BG", studioBg);
	iFps = settingsini.GetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);
	horiHd = settingsini.GetInt("SAVVY-MANAGER", "HORI_HD", horiHd);

	ss1Region = settingsini.GetInt("SS1", "REGION", ss1Region);
	ss2Region = settingsini.GetInt("SS2", "REGION", ss2Region);
	ss3Region = settingsini.GetInt("SS3", "REGION", ss3Region);
	ss4Region = settingsini.GetInt("SS4", "REGION", ss4Region);

	currentMusicPack = settingsini.GetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);
}

void saveSettings(void) {
	CIniFile settingsini(settingsIni);

	//settingsini.SetInt("SAVVY-MANAGER", "STUDIO_BG", studioBg);
	settingsini.SetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);

	settingsini.SetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);

	settingsini.SaveIniFileModified(settingsIni);
}

void Play_Music(void) {
	if (musicPlaying && !musicLoopPlaying) {
		musicLoopDelay++;
		if (musicLoopDelay>60 && !ndspChnIsPlaying(0)) {
			music_loop->play();
			musicLoopPlaying = true;
		}
	}
	if (!musicPlaying && dspfirmfound) {
		music->play();
		musicPlaying = true;
	}
}

void musLogos(void) {
	if (!dspfirmfound) return;
	mus_logos->stop();
	mus_logos->play();
}

void sndSelect(void) {
	if (!dspfirmfound) return;
	sfx_select->stop();
	sfx_select->play();
}

void sndBack(void) {
	if (!dspfirmfound) return;
	sfx_back->stop();
	sfx_back->play();
}

void sndHighlight(void) {
	if (!dspfirmfound) return;
	sfx_highlight->stop();
	sfx_highlight->play();
}

u8 sysRegion = CFG_REGION_USA;
u64 appID = 0;
int orgHighlightedGame = 1;
int highlightedGame = 1;

bool doubleSpeed = false;
float bg_xPos = 0.0f;
float bg_yPos = 0.0f;
bool showCursor = false;
int cursorAlpha = 0;

int ss1Logo = gameSelSprites_title1_idx;
int ss2Logo = gameSelSprites_title2_idx;
int ss3Logo = gameSelSprites_title3_idx;
int ss4Logo = gameSelSprites_title4_idx;
int ss1LogoXpos = 0;
int ss2LogoXpos = 0;
int ss4LogoXpos = 0;

u32 hDown = 0;
u32 hDownRepeat = 0;
u32 hHeld = 0;
touchPosition touch;

bool touchingBackButton(void) {
	return (touch.px >= 7 && touch.px < 7+40 && touch.py >= 197 && touch.py < 197+44);
}

void renderTopScreenSubPixels(void) {
	shiftBySubPixel = true;
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	Gui::clearTextBufs();
	Gui::DrawScreen();
	C3D_FrameEnd(0);
	shiftBySubPixel = false;
}

bool ss2SaveFound = false;
bool ss3SaveFound = false;
bool ss4SaveFound = false;

static bool runThreads = true;

void controlThread(void) {
	while (runThreads) {
		svcWaitSynchronization(threadRequest, U64_MAX);
		Gui::ScreenLogic(hDown, hDownRepeat, hHeld, touch, true); // Call the logic of the current screen here.
		svcClearEvent(threadRequest);
	}
}

int main()
{
	amInit();
	romfsInit();

	u8 consoleModel = 0;

	Result res = cfguInit();
	if (R_SUCCEEDED(res)) {
		CFGU_SecureInfoGetRegion(&sysRegion);
		CFGU_GetSystemModel(&consoleModel);
		cfguExit();
	}

	s64 isCitra = 0;
	svcGetSystemInfo(&isCitra, 0x20000, 0); // Check if running on Citra

	aptInit();
	APT_GetProgramID(&appID);
	aptExit();

	gfxInitDefault();
	loadSettings();
	gfxSetWide(!isCitra && horiHd && consoleModel != 3);	// Enable 800x240 mode for non-O2DS consoles. Improves clarity in graphics.

	hidSetRepeatParameters(25, 5);

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
		mus_logos = new sound("romfs:/sounds/rocketRobz.wav", 0, false);
		music = new sound("romfs:/sounds/music_start.wav", 0, false);
		music_loop = new sound("romfs:/sounds/music_loop.wav", 1, true);
		sfx_select = new sound("romfs:/sounds/select.wav", 2, false);
		sfx_back = new sound("romfs:/sounds/back.wav", 3, false);
		sfx_highlight = new sound("romfs:/sounds/highlight.wav", 4, false);
	}

	u32 ss2Id[4] = {0x000A9100, 0x000A9000, 0x0005D100, 0x000C4F00};
	u32 ss3Id[3] = {0x00196500, 0x0016A100, 0x0012D800};
	u32 ss4Id[3] = {0x00001C25, 0x00001C26, 0x000019F6};

	if (ss1Region < 0 || ss1Region >= 6) {
		ss1Region = sysRegion;
	}

	switch (ss1Region) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss1Logo = gameSelSprites_title1_E_idx;
			ss1LogoXpos = 32;
			break;
		case CFG_REGION_JPN:
			ss1Logo = gameSelSprites_title1_J_idx;
			break;
		case CFG_REGION_KOR:
			ss1Logo = gameSelSprites_title1_K_idx;
			ss1LogoXpos = 64;
			break;
		default:
			break;
	}

	if (ss2Region < 0 || ss2Region >= 6) {
		ss2Region = sysRegion;
	}

	switch (ss2Region) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss2Id[0] = 0x000A9000;
			ss2Id[1] = 0x000A9100; // Fallback: USA
			break;
		case CFG_REGION_JPN:
			ss2Id[0] = 0x0005D100;
			ss2Id[1] = 0x000C4F00; // Fallback: KOR
			ss2Id[2] = 0x000A9100; // Fallback: USA
			ss2Id[3] = 0x000A9000; // Fallback: EUR/AUS
			break;
		case CFG_REGION_KOR:
			ss2Id[0] = 0x000C4F00;
			ss2Id[1] = 0x0005D100; // Fallback: JPN
			ss2Id[2] = 0x000A9100; // Fallback: USA
			ss2Id[3] = 0x000A9000; // Fallback: EUR/AUS
			break;
		default:
			break;
	}

	if (ss3Region < 0 || ss3Region >= 6) {
		ss3Region = sysRegion;
	}

	switch (ss3Region) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss3Id[0] = 0x0016A100;
			ss3Id[1] = 0x00196500; // Fallback: USA
			break;
		case CFG_REGION_JPN:
		case CFG_REGION_KOR:
			ss3Id[0] = 0x0012D800;
			ss3Id[1] = 0x00196500; // Fallback: USA
			ss3Id[2] = 0x0016A100; // Fallback: EUR/AUS
			break;
		default:
			break;
	}

	if (ss4Region < 0 || ss4Region >= 6) {
		ss4Region = sysRegion;
	}

	switch (ss4Region) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss4Id[0] = 0x00001C26;
			ss4Id[1] = 0x00001C25; // Fallback: USA
			break;
		case CFG_REGION_JPN:
		case CFG_REGION_KOR:
			ss4Id[0] = 0x000019F6;
			ss4Id[1] = 0x00001C25; // Fallback: USA
			ss4Id[2] = 0x00001C26; // Fallback: EUR/AUS
			break;
		default:
			break;
	}

	u32 path2[3] = {MEDIATYPE_SD, ss2Id[0], 0x00040000};
	u32 path2card[3] = {MEDIATYPE_GAME_CARD, ss2Id[0], 0x00040000};
	u32 path3[3] = {MEDIATYPE_SD, ss3Id[0], 0x00040000};
	u32 path3card[3] = {MEDIATYPE_GAME_CARD, ss3Id[0], 0x00040000};
	u32 path4[3] = {MEDIATYPE_SD, ss4Id[0], 0};

	for (int i = 0; i < 4; i++) {
		path2[1] = ss2Id[i];
		path2card[1] = ss2Id[i];
		res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path2}, "ss2");	// Read from digital version
		if (R_FAILED(res)) {
			res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path2card}, "ss2");	// Read from game card
			if (R_SUCCEEDED(res)) {
				break;
			}
		} else {
			break;
		}
	}
	switch (path2[1]) {
		case 0x000A9100:
			saveRegion[1] = CFG_REGION_USA;
			break;
		case 0x000A9000:
			saveRegion[1] = CFG_REGION_EUR;
			break;
		case 0x0005D100:
			saveRegion[1] = CFG_REGION_JPN;
			break;
		case 0x000C4F00:
			saveRegion[1] = CFG_REGION_KOR;
			break;
	}
	if (R_SUCCEEDED(res)) {
		switch (saveRegion[1]) {
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
			case CFG_REGION_KOR:
				// Girls Style: Paesyeon Lideo Seon-eon! folders
				mkdir("sdmc:/luma/titles/00040000000C4F00", 0777);
				mkdir("sdmc:/luma/titles/00040000000C4F00/romfs", 0777);
				mkdir("sdmc:/luma/titles/00040000000C4F00/romfs/Common", 0777);
				mkdir("sdmc:/luma/titles/00040000000C4F00/romfs/Common/Sound", 0777);
				break;
		}
		ss2SaveFound = (access(ss2SavePath, F_OK) == 0);
	}

	for (int i = 0; i < 3; i++) {
		path3[1] = ss3Id[i];
		path3card[1] = ss3Id[i];
		res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path3}, "ss3");	// Read from digital version
		if (R_FAILED(res)) {
			res = archiveMount(ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path3card}, "ss3");	// Read from game card
			if (R_SUCCEEDED(res)) {
				break;
			}
		} else {
			break;
		}
	}
	switch (path3[1]) {
		case 0x00196500:
			saveRegion[2] = CFG_REGION_USA;
			break;
		case 0x0016A100:
			saveRegion[2] = CFG_REGION_EUR;
			break;
		case 0x0012D800:
			saveRegion[2] = CFG_REGION_JPN;
			break;
	}
	if (R_SUCCEEDED(res)) {
		ss3SaveFound = (access(ss3SavePath, F_OK) == 0);
	}

	for (int i = 0; i < 3; i++) {
		path4[1] = ss4Id[i];
		res = FSUSER_OpenArchive(&archive4, ARCHIVE_EXTDATA, {PATH_BINARY, 12, path4});
		if (R_SUCCEEDED(res)) {
			break;
		}
	}
	switch (path4[1]) {
		case 0x00001C25:
			saveRegion[3] = CFG_REGION_USA;
			break;
		case 0x00001C26:
			saveRegion[3] = CFG_REGION_EUR;
			break;
		case 0x000019F6:
			saveRegion[3] = CFG_REGION_JPN;
			break;
	}
	if (R_SUCCEEDED(res)) {
		switch (saveRegion[3]) {
			default:
				// Style Savvy: Styling Star folders
				mkdir("sdmc:/luma/titles/00040000001C2500", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2500/romfs", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2500/romfs/USA", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2500/romfs/USA/Common", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2500/romfs/USA/Common/Scene", 0777);
				readDanceCpk();
				break;
			case CFG_REGION_EUR:
			case CFG_REGION_AUS:
				// New Style Boutique 3: Styling Star folders
				mkdir("sdmc:/luma/titles/00040000001C2600", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2600/romfs", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2600/romfs/EUR", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2600/romfs/EUR/Common", 0777);
				mkdir("sdmc:/luma/titles/00040000001C2600/romfs/EUR/Common/Scene", 0777);
				readDanceCpk();
				break;
			case CFG_REGION_JPN:
				// Girls Mode 4: Star Stylist
				mkdir("sdmc:/luma/titles/000400000019F600", 0777);
				mkdir("sdmc:/luma/titles/000400000019F600/romfs", 0777);
				mkdir("sdmc:/luma/titles/000400000019F600/romfs/JPN", 0777);
				mkdir("sdmc:/luma/titles/000400000019F600/romfs/JPN/Common", 0777);
				mkdir("sdmc:/luma/titles/000400000019F600/romfs/JPN/Common/Scene", 0777);
				readDanceCpk();
				break;
		}
		ss4SaveFound = foundSS4Save();
	}

	ss3DLCharactersBackedUp = (access("sdmc:/3ds/SavvyManager/SS3/dlCharacters.bak", F_OK) == 0);

	switch (saveRegion[1]) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss2Logo = gameSelSprites_title2_E_idx;
			ss2LogoXpos = 32;
			break;
		case CFG_REGION_JPN:
			ss2Logo = gameSelSprites_title2_J_idx;
			break;
		case CFG_REGION_KOR:
			ss2Logo = gameSelSprites_title2_K_idx;
			break;
		default:
			break;
	}

	switch (saveRegion[2]) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss3Logo = gameSelSprites_title3_E_idx;
			break;
		case CFG_REGION_JPN:
		case CFG_REGION_KOR:
			ss3Logo = gameSelSprites_title3_J_idx;
			break;
		default:
			break;
	}

	switch (saveRegion[3]) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			ss4Id[0] = 0x00001C26;
			ss4Id[1] = 0x00001C25; // Fallback: USA
			ss4Logo = gameSelSprites_title4_E_idx;
			ss4LogoXpos = 32;
			break;
		case CFG_REGION_JPN:
		case CFG_REGION_KOR:
			ss4Id[0] = 0x000019F6;
			ss4Id[1] = 0x00001C25; // Fallback: USA
			ss4Id[2] = 0x00001C26; // Fallback: EUR/AUS
			ss4Logo = gameSelSprites_title4_J_idx;
			break;
		default:
			break;
	}

	sprintf(verText, "Ver. %i.%i.%i", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	C3D_FrameRate(iFps);

	Gui::setScreen(std::make_unique<RocketRobz>(), false); // Set screen to RocketRobz's screen.
	svcCreateEvent(&threadRequest,(ResetType)0);
	createThread((ThreadFunc)controlThread);
	musLogos();

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		// Scan hid shared memory for input events
		hidScanInput();

		hDown = hidKeysDown();
		hDownRepeat = hidKeysDownRepeat();
		hHeld = hidKeysHeld();

		hidTouchRead(&touch);

		if (musicPlayStarted) {
			Play_Music();
		}

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
			if (delay > iFps*11) {
				Gui::setScreen(std::make_unique<GameSelect>(), true); // Set after delay to the game select screen.
				isInit = false;
			}
		}

		// Scroll background
		switch (iFps) {
			default:
				bg_xPos += 0.3*(1+doubleSpeed);
				bg_yPos -= 0.3*(1+doubleSpeed);
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

		if (hDown || hDownRepeat) {
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
				fadeFPS = 6*(1+doubleSpeed);
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
	//delete mus_logos;
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