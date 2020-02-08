#include "common.hpp"

#include <unistd.h>

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet bgSprite;
static C2D_SpriteSheet chracterSprite;
static bool dochracterSpriteFree = false;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

Result GFX::loadSheets() {
	sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	bgSprite   = C2D_SpriteSheetLoad("romfs:/gfx/bgNight_loversBell.t3x");
	return 0;
}

Result GFX::unloadSheets() {
	C2D_SpriteSheetFree(sprites);
	C2D_SpriteSheetFree(bgSprite);
	return 0;
}

bool GFX::loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
	if (dochracterSpriteFree) {
		C2D_SpriteSheetFree(chracterSprite);
	}
	if (access(t3xPathAllSeasons, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathAllSeasons);
		dochracterSpriteFree = true;
		return true;
	} else {
		dochracterSpriteFree = false;
	}
	if (access(t3xPathOneSeason, F_OK) == 0) {
		chracterSprite = C2D_SpriteSheetLoad(t3xPathOneSeason);
		dochracterSpriteFree = true;
		return true;
	} else {
		dochracterSpriteFree = false;
	}
	return false;
}

void GFX::showBgSprite(int zoomIn) {
	Gui::DrawSprite(bgSprite, 0, 0, -(240*zoomIn));
}

void GFX::showCharSprite(int zoomIn, int fadeAlpha) {
	if (fadeAlpha == 255) {
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(chracterSprite, 0), 0, -(240*zoomIn), 0.5f);
	} else {
		C2D_ImageTint tint;
		C2D_SetImageTint(&tint, C2D_TopLeft, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_TopRight, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_BotLeft, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_SetImageTint(&tint, C2D_BotRight, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(chracterSprite, 0), 0, -(240*zoomIn), 0.5f, &tint);
	}
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, img), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void GFX::DrawSpriteBlend(int key, float x, float y, u32 color, float ScaleX, float ScaleY) {
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint, ScaleX, ScaleY);
}

void GFX::drawCursor(void) {
	if (cursorAlpha == 255) {
		DrawSprite(sprites_cursor_idx, cursorX, cursorY);
	} else if (cursorAlpha > 0) {
		DrawSpriteBlend(sprites_cursor_idx, cursorX, cursorY, C2D_Color32(255, 255, 255, cursorAlpha));
	}
}