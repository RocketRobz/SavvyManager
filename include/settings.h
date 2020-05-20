#ifndef SETTINGS_H
#define SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string>

extern int iFps;
extern std::string currentMusicPack;

extern void loadSettings(void);
extern void saveSettings(void);

#ifdef __cplusplus
}
#endif

#endif	// SETTINGS_H