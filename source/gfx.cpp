#include "common.hpp"

#include <unistd.h>

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet gameSelSprites;
static C2D_SpriteSheet gameShotSprites;
static C2D_SpriteSheet bgSprite;
static C2D_SpriteSheet chracterSprite;
static bool doChracterSpriteFree = false;
static bool doBgSpriteFree = false;

extern int studioBg;
extern int cinemaWide;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

Result GFX::loadSheets() {
	sprites			= C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	gameSelSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameSelSprites.t3x");
	gameShotSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameShotSprites.t3x");
	GFX::loadBgSprite();
	return 0;
}

Result GFX::unloadSheets() {
	C2D_SpriteSheetFree(sprites);
	C2D_SpriteSheetFree(gameSelSprites);
	C2D_SpriteSheetFree(gameShotSprites);
	if (doBgSpriteFree) {
		C2D_SpriteSheetFree(bgSprite);
	}
	if (doChracterSpriteFree) {
		C2D_SpriteSheetFree(chracterSprite);
	}
	return 0;
}

void GFX::loadBgSprite(void) {
	if (doBgSpriteFree) {
		C2D_SpriteSheetFree(bgSprite);
	}
	const char* bgPath;
	switch (studioBg) {
		case 0:
		default:
			bgPath = "romfs:/gfx/bg_blue.t3x";
			break;
		case 1:
			bgPath = "romfs:/gfx/bgNight_loversBell.t3x";
			break;
		case 2:
			bgPath = "romfs:/gfx/bgNight_bougainville.t3x";
			break;
	}
	bgSprite		= C2D_SpriteSheetLoad(bgPath);
	doBgSpriteFree	= true;
}

bool GFX::loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
	if (doChracterSpriteFree) {
		C2D_SpriteSheetFree(chracterSprite);
	}
	if (access(t3xPathAllSeasons, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathAllSeasons);
		doChracterSpriteFree = true;
		return true;
	} else {
		doChracterSpriteFree = false;
	}
	if (access(t3xPathOneSeason, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathOneSeason);
		doChracterSpriteFree = true;
		return true;
	} else {
		doChracterSpriteFree = false;
	}
	return false;
}

void GFX::showBgSprite(int zoomIn) {
	int yPos = -(240*zoomIn);
	if (cinemaWide) yPos -= 16;

	Gui::DrawSprite(bgSprite, 0, 0, yPos);
}

void GFX::showCharSprite(int zoomIn, int fadeAlpha) {
	int yPos = -((cinemaWide ? 168 : 240)*zoomIn);
	if (cinemaWide) yPos += 36;

	C2D_Image image = C2D_SpriteSheetGetImage(chracterSprite, 0);
	C3D_TexSetFilter(image.tex, GPU_LINEAR, GPU_LINEAR);

	if (fadeAlpha == 255) {
		C2D_DrawImageAt(image, (cinemaWide ? 60 : 0), yPos, 0.5f, NULL, (cinemaWide ? 0.7f : 1), (cinemaWide ? 0.7f : 1));
	} else {
		C2D_ImageTint tint;
		C2D_SetImageTint(&tint, C2D_TopLeft, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_TopRight, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_BotLeft, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_BotRight, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_DrawImageAt(image, (cinemaWide ? 60 : 0), yPos, 0.5f, &tint, (cinemaWide ? 0.7f : 1), (cinemaWide ? 0.7f : 1));
	}
}

void GFX::DrawGameSelSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(gameSelSprites, img), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawGameShotSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(gameShotSprites, img), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY, GPU_TEXTURE_FILTER_PARAM filter) {
	C2D_Image image = C2D_SpriteSheetGetImage(sprites, img);
	C3D_TexSetFilter(image.tex, filter, filter);

	C2D_DrawImageAt(image, x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX, float ScaleY, GPU_TEXTURE_FILTER_PARAM filter) {
	C2D_Image image = C2D_SpriteSheetGetImage(sprites, img);
	C3D_TexSetFilter(image.tex, filter, filter);

	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(image, x, y, 0.5f, &tint, ScaleX, ScaleY);
}

void GFX::drawCursor(int cX, int cY) {
	if (cursorAlpha == 255) {
		DrawSprite(sprites_cursor_idx, cX, cY);
	} else if (cursorAlpha > 0) {
		DrawSpriteBlend(sprites_cursor_idx, cX, cY, C2D_Color32(255, 255, 255, cursorAlpha));
	}
}