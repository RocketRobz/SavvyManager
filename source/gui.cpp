/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#include <unistd.h>


C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet bgSprite;
static C2D_SpriteSheet chracterSprite;
static bool dochracterSpriteFree = false;
bool currentScreen = false; // False -> Bottom, True -> Top.
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
	bgSprite   = C2D_SpriteSheetLoad("romfs:/gfx/bgNight_loversBell.t3x");
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

// Select, on which Screen should be drawn.
void Gui::setDraw(C3D_RenderTarget * screen)
{
	C2D_SceneBegin(screen);
	currentScreen = screen == top ? 1 : 0;
}

void Gui::sprite(int key, float x, float y, float ScaleX, float ScaleY) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, NULL, ScaleX, ScaleY);
}

void Gui::showBgSprite(int zoomIn) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(bgSprite, 0), 0, -(240*zoomIn), 0.5f);
}

void Gui::showCharSprite(int zoomIn, int fadeAlpha) {
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

void Gui::Draw_ImageBlend(int key, float x, float y, u32 color) {
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint);
}

void Gui::DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth) {
	Gui::DrawString((currentScreen ? 200 : 160)+x-((maxWidth == 0 ? (int)Gui::GetStringWidth(size, Text) : std::min(maxWidth, (int)Gui::GetStringWidth(size, Text)))/2), y, size, color, Text, maxWidth);
}

// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth) {
	C2D_Text c2d_text;
	C2D_TextParse(&c2d_text, sizeBuf, Text.c_str());
	C2D_TextOptimize(&c2d_text);
	if(maxWidth == 0) {
		C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
	} else {
		C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, std::min(size, size*(maxWidth/Gui::GetStringWidth(size, Text))), size, color);
	}
}

// Get String or Text Width.
float Gui::GetStringWidth(float size, std::string Text) {
	float width = 0;
	GetStringSize(size, &width, NULL, Text);
	return width;
}

// Get String or Text Size.
void Gui::GetStringSize(float size, float *width, float *height, std::string Text) {
	C2D_Text c2d_text;
	C2D_TextParse(&c2d_text, sizeBuf, Text.c_str());
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

// Get String or Text Height.
float Gui::GetStringHeight(float size, std::string Text) {
	float height = 0;
	GetStringSize(size, NULL, &height, Text.c_str());
	return height;
}

bool Gui::Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}