#ifndef SCREEN_MODE_H
#define SCREEN_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

enum ScreenMode {
	SCREEN_MODE_EXIT = -1,				// Exit homebrew
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_SETTINGS = 2,			// Settings
	SCREEN_MODE_WHAT_TO_DO = 3,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 4,	// Change character
	SCREEN_MODE_CHANGE_MUSIC = 5,		// Change music
	SCREEN_MODE_CHANGE_EMBLEM = 6		// Change emblem
};

#ifdef __cplusplus
}
#endif

#endif // SCREEN_MODE_H
