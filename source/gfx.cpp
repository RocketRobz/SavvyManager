#include "common.hpp"

#include <unistd.h>

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet gameSelSprites;
static C2D_SpriteSheet gameShotSprites;
static C2D_SpriteSheet gameBgSprites;
static C2D_SpriteSheet chracterSprite;
static bool chracterSpriteLoaded = false;

extern int studioBg;
extern int cinemaWide;
extern int iFps;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

bool shiftBySubPixel = false;

bool gameSelGraphicsLoaded = false;

Result GFX::loadSheets() {
	sprites			= C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	return 0;
}

Result GFX::unloadSheets() {
	C2D_SpriteSheetFree(sprites);
	if (gameSelGraphicsLoaded) {
		C2D_SpriteSheetFree(gameSelSprites);
		C2D_SpriteSheetFree(gameShotSprites);
		C2D_SpriteSheetFree(gameBgSprites);
	}
	if (chracterSpriteLoaded) {
		C2D_SpriteSheetFree(chracterSprite);
	}
	return 0;
}

void GFX::loadGameSelSheets() {
	if (gameSelGraphicsLoaded) return;

	gameSelSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameSelSprites.t3x");
	gameShotSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameShotSprites.t3x");
	gameBgSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameBgSprites.t3x");
	gameSelGraphicsLoaded = true;
}

void GFX::unloadGameSelSheets() {
	if (!gameSelGraphicsLoaded) return;

	C2D_SpriteSheetFree(gameSelSprites);
	C2D_SpriteSheetFree(gameShotSprites);
	C2D_SpriteSheetFree(gameBgSprites);
	gameSelGraphicsLoaded = false;
}

bool GFX::loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
	if (chracterSpriteLoaded) {
		C2D_SpriteSheetFree(chracterSprite);
	}
	if (access(t3xPathAllSeasons, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathAllSeasons);
		chracterSpriteLoaded = true;
		return true;
	} else {
		chracterSpriteLoaded = false;
	}
	if (access(t3xPathOneSeason, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathOneSeason);
		chracterSpriteLoaded = true;
		return true;
	} else {
		chracterSpriteLoaded = false;
	}
	return false;
}

void GFX::showCharSprite(int zoomIn, int fadeAlpha, bool lightingEffects) {
	int yPos = -((cinemaWide ? 168 : 240)*zoomIn);
	if (cinemaWide) yPos += 36;

	C2D_Image image = C2D_SpriteSheetGetImage(chracterSprite, 0);
	if (!gfxIsWide() || cinemaWide) {
		C3D_TexSetFilter(image.tex, GPU_LINEAR, GPU_LINEAR);
	}

	C2D_ImageTint tint;
	if (fadeAlpha == 255) {
		C2D_PlainImageTint(&tint, C2D_Color32(255, 255, 255, 255), 0);
	} else {
		C2D_PlainImageTint(&tint, C2D_Color32(255, 255, 255, fadeAlpha), 1);
	}
	C2D_DrawImageAt(image, (cinemaWide ? 60 : 0), yPos-(shiftBySubPixel ? 0.5f : 0), 0.5f, &tint, (cinemaWide ? 0.35f : 0.5), (cinemaWide ? 0.7f : 1));
}

void GFX::DrawGameSelSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(gameSelSprites, img), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawGameShotSprite(int img, int x, int y) {
	C2D_Image image = C2D_SpriteSheetGetImage(gameShotSprites, img);
	if (!gfxIsWide()) {
		C3D_TexSetFilter(image.tex, GPU_LINEAR, GPU_LINEAR);
	}

	C2D_DrawImageAt(image, x, y-(shiftBySubPixel ? 0.5f : 0), 0.5f, NULL, 0.5, 1);
}

void GFX::DrawGameBgSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(gameBgSprites, img), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY, GPU_TEXTURE_FILTER_PARAM filter) {
	float yPos = y;
	if (shiftBySubPixel && img != sprites_logo_savvymanager_idx) {
		yPos -= 0.5f;
	}

	C2D_Image image = C2D_SpriteSheetGetImage(sprites, img);
	C3D_TexSetFilter(image.tex, filter, filter);

	C2D_DrawImageAt(image, x, yPos, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX, float ScaleY, GPU_TEXTURE_FILTER_PARAM filter) {
	C2D_Image image = C2D_SpriteSheetGetImage(sprites, img);
	C3D_TexSetFilter(image.tex, filter, filter);

	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(image, x, y-(shiftBySubPixel ? 0.5f : 0), 0.5f, &tint, ScaleX, ScaleY);
}

void GFX::drawCursor(int cX, int cY) {
	if (cursorAlpha == 255) {
		DrawSprite(sprites_cursor_idx, cX, cY);
	} else if (cursorAlpha > 0) {
		DrawSpriteBlend(sprites_cursor_idx, cX, cY, C2D_Color32(255, 255, 255, cursorAlpha));
	}
}