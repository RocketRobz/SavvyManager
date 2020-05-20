#include <3ds.h>
#include <string>

#define settingsIni "sdmc:/3ds/SavvyManager/settings.ini"

#include "inifile.h"

int iFps = 60;
std::string currentMusicPack = "";

void loadSettings(void) {
	CIniFile settingsini(settingsIni);

	iFps = settingsini.GetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);

	currentMusicPack = settingsini.GetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);
}

void saveSettings(void) {
	CIniFile settingsini(settingsIni);

	settingsini.SetInt("SAVVY-MANAGER", "FRAME_RATE", iFps);

	settingsini.SetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);

	settingsini.SaveIniFile(settingsIni);
}
