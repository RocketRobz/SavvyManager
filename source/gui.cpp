/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "gui.hpp"
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>


C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet chracterSprite;
static bool dochracterSpriteFree = false;
C2D_TextBuf sizeBuf;
C2D_Font systemFont;

void Gui::clearTextBufs(void) {
	C2D_TextBufClear(sizeBuf);
}

Result Gui::init(void) {
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	sizeBuf = C2D_TextBufNew(4096);
	sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	systemFont = C2D_FontLoadSystem(CFG_REGION_USA);
	return 0;
}

bool Gui::loadCharSprite(const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
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

void Gui::exit(void) {
	if (sprites) {
		C2D_SpriteSheetFree(sprites);
	}
	C2D_TextBufDelete(sizeBuf);
	C2D_Fini();
	C3D_Fini();
}

void set_screen(C3D_RenderTarget * screen) {
	C2D_SceneBegin(screen);
}

void Gui::sprite(int key, float x, float y) {
	if (key == sprites_res_null_idx) {
		return;
	} else { // standard case
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f);
	}
}

void Gui::spriteScale(int key, float x, float y, float scaleX, float scaleY) {
	if (key == sprites_res_null_idx) {
		return;
	} else { // standard case
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, NULL, scaleX, scaleY);
	}
}

void Gui::charSprite(float x, float y) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(chracterSprite, 0), x, y, 0.5f);
}

void Gui::Draw_ImageBlend(int key, float x, float y, u32 color) {
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint);
}

void Draw_EndFrame(void) {
	C2D_TextBufClear(sizeBuf);
	C3D_FrameEnd(0);
}

void Draw_Text(float x, float y, float size, u32 color, const char *text) {
	C2D_Text c2d_text;
	C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, text);
	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
}

void Draw_Textf(float x, float y, float size, u32 color, const char* text, ...) {
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	Draw_Text(x, y, size, color, buffer);
	va_end(args);
}

void Draw_GetTextSize(float size, float *width, float *height, const char *text) {
	C2D_Text c2d_text;
	C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, text);
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

float Draw_GetTextWidth(float size, const char *text) {
	float width = 0;
	Draw_GetTextSize(size, &width, NULL, text);
	return width;
}

float Draw_GetTextHeight(float size, const char *text) {
	float height = 0;
	Draw_GetTextSize(size, NULL, &height, text);
	return height;
}

bool Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}