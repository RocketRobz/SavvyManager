#ifndef SCREEN_VARS_H
#define SCREEN_VARS_H

// List all common screen variables here.
extern u8 sysRegion;
extern bool showCursor;
extern float bg_xPos;
extern float bg_yPos;
extern int cursorAlpha;
extern int studioBg;
extern bool cinemaWide;
extern int iFps;
extern std::string currentMusicPack;
extern int highlightedGame;

// List all common screen functions here.
extern void sndBack(void);
extern void sndSelect(void);
extern void sndHighlight(void);
extern bool touchingBackButton(void);

#endif // SCREEN_VARS_H