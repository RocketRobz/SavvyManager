#include <3ds.h>
#include <string>

#define settingsIni "sdmc:/3ds/SavvyManager/settings.ini"

#include "inifile.h"

std::string currentMusicPack = "";

void loadSettings(void) {
	CIniFile settingsini(settingsIni);

	currentMusicPack = settingsini.GetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);
}

void saveSettings(void) {
	CIniFile settingsini(settingsIni);

	settingsini.SetString("SS2", "CURRENT_MUSIC_PACK", currentMusicPack);

	settingsini.SaveIniFile(settingsIni);
}
