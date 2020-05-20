#ifndef GFX_HPP
#define GFX_HPP

#include "sprites.h"
#include "gameSelSprites.h"

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

namespace GFX
{
	// Load & Unload default sheets.
	Result loadSheets();
	Result unloadSheets();

	// Sprirte Functions.
	bool loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason);
	void showBgSprite(int zoomIn);
	void showCharSprite(int zoomIn, int fadeAlpha);
	void DrawGameSelSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawSpriteBlend(int key, float x, float y, u32 color, float ScaleX = 1, float ScaleY = 1);

	// Draw Cursor.
	void drawCursor(void);
}

#endif