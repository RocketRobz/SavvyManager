#ifndef GFX_HPP
#define GFX_HPP

#include "sprites.h"
#include "gameSelSprites.h"
#include "gameShotSprites.h"
#include "gameBgSprites.h"

#include <3ds.h>
#include <citro2d.h>

// Colors.
#define TRANSPARENT C2D_Color32(0, 0, 0, 0)
#define BLACK C2D_Color32(0, 0, 0, 255)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define GRAY C2D_Color32(127, 127, 127, 255)
#define BLUE C2D_Color32(0, 0, 255, 255)
#define GREEN C2D_Color32(0, 255, 0, 255)
#define RED C2D_Color32(255, 0, 0, 255)
#define MSG_BUTTONTEXT C2D_Color32(181, 48, 0, 255)

#define TIME C2D_Color32(16, 0, 0, 223)

typedef u32 Color;

extern bool shiftBySubPixel;

namespace GFX {
	// Load & Unload default sheets.
	Result loadSheets();
	Result unloadSheets();

	// Sprite Functions.
	bool loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason);
	void loadBgSprite(void);
	void showBgSprite(int zoomIn);
	void showCharSprite(int zoomIn, int fadeAlpha, bool lightingEffects = false);
	void DrawGameSelSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawGameShotSprite(int img, int x, int y);
	void DrawGameBgSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1, GPU_TEXTURE_FILTER_PARAM filter = GPU_NEAREST);
	void DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX = 1, float ScaleY = 1, GPU_TEXTURE_FILTER_PARAM filter = GPU_NEAREST);

	// Draw Cursor.
	void drawCursor(int cX, int cY);
}

#endif