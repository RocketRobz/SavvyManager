#include "common.hpp"

#include <unistd.h>

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet gameSelSprites;
static C2D_SpriteSheet gameShotSprites;
static C2D_SpriteSheet gameBgSprites;
static C2D_SpriteSheet bgSprite[4];
static C2D_SpriteSheet chracterSprite;
static bool doChracterSpriteFree = false;
static bool doBgSpriteFree[4] = {false};

extern int studioBg;
extern int cinemaWide;
extern int iFps;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

static bool animateBg = false;
static int bgAnimationFrame = 0;
static int bgAnimationCurrent = 0;
static int bgAnimationTime = 0;
//static int bgAnimationDelay = 0;
static int bgAnimation[8] = {100};

static int timeOutside = 0;	// 0 == Day, 1 == Sunset, 2 == Night

bool shiftBySubPixel = false;

Result GFX::loadSheets() {
	sprites			= C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	gameSelSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameSelSprites.t3x");
	gameShotSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameShotSprites.t3x");
	gameBgSprites	= C2D_SpriteSheetLoad("romfs:/gfx/gameBgSprites.t3x");
	GFX::loadBgSprite();
	return 0;
}

Result GFX::unloadSheets() {
	C2D_SpriteSheetFree(sprites);
	C2D_SpriteSheetFree(gameSelSprites);
	C2D_SpriteSheetFree(gameShotSprites);
	C2D_SpriteSheetFree(gameBgSprites);
	for (int i = 0; i < 4; i++) {
		if (doBgSpriteFree[i]) {
			C2D_SpriteSheetFree(bgSprite[i]);
		}
	}
	return 0;
}

void GFX::loadBgSprite(void) {
	for (int i = 0; i < 4; i++) {
		if (doBgSpriteFree[i]) {
			C2D_SpriteSheetFree(bgSprite[i]);
		}
	}

	timeOutside = 0;

	const char* bgPath;
	time_t t = time(0);
	int hour = localtime(&t)->tm_hour;

	switch (studioBg) {
		case 0:
		default:
			bgPath = "romfs:/gfx/bg_blue.t3x";
			break;
		case 1:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_loversBell.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_loversBell.t3x";
				timeOutside = 2;
			}
			break;
		case 2:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_bougainville.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_bougainville.t3x";
				timeOutside = 2;
			}
			break;
		case 3:
			bgPath = "romfs:/gfx/bg_nin10Pro.t3x";
			break;
		case 4:
			bgPath = "romfs:/gfx/bg_beautician4.t3x";
			break;
		case 5:
			bgPath = "romfs:/gfx/bg_hairSalon4.t3x";
			break;
		case 6:
			bgPath = "romfs:/gfx/bg_celestialHotel.t3x";
			break;
		case 7:
			bgPath = "romfs:/gfx/bg_liveMusicClub4.t3x";
			break;
		case 8:
			bgPath = "romfs:/gfx/bg_menswearShop.t3x";
			break;
		case 9:
			bgPath = "romfs:/gfx/bg_VIP.t3x";
			break;
		case 10:
			bgPath = "romfs:/gfx/bg_restauraunt4.t3x";
			break;
		case 11:
			bgPath = "romfs:/gfx/bg_cinema.t3x";
			break;
		case 12:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_tropicaBeach_0.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_tropicaBeach.t3x";
				timeOutside = 2;
			}
			break;
		case 13:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_primrosePark.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_primrosePark.t3x";
				timeOutside = 2;
			}
			break;
		case 14:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_cafe3.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_cafe3.t3x";
				timeOutside = 2;
			}
			break;
		case 15:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_mapleCrescent.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_mapleCrescent.t3x";
				timeOutside = 2;
			}
			break;
		case 16:
			bgPath = "romfs:/gfx/bg_white.t3x";
			break;
		case 17:
			bgPath = "romfs:/gfx/bg_yellow.t3x";
			break;
		case 18:
			bgPath = "romfs:/gfx/bg_snowflakes.t3x";
			break;
		case 19:
			bgPath = "romfs:/gfx/bg_pinkShine.t3x";
			break;
		case 20:
			bgPath = "romfs:/gfx/bg_beigeWithCircles.t3x";
			break;
		case 21:
			bgPath = "romfs:/gfx/bg_gradientPurple.t3x";
			break;
		case 22:
			bgPath = "romfs:/gfx/bg_greenBirds.t3x";
			break;
		case 23:
			bgPath = "romfs:/gfx/bg_circlePattern.t3x";
			break;
		case 24:
			bgPath = "romfs:/gfx/bg_circleWindows.t3x";
			break;
		case 25:
			bgPath = "romfs:/gfx/bg_bigHearts.t3x";
			break;
		case 26:
			bgPath = "romfs:/gfx/bg_smallHearts.t3x";
			break;
		case 27:
			bgPath = "romfs:/gfx/bg_park2Spring.t3x";
			break;
		case 28:
			bgPath = "romfs:/gfx/bg_park2Summer.t3x";
			break;
		case 29:
			bgPath = "romfs:/gfx/bg_park2Fall.t3x";
			break;
		case 30:
			bgPath = "romfs:/gfx/bg_park2Winter.t3x";
			break;
		case 31:
			bgPath = "romfs:/gfx/bg_downtownSpring.t3x";
			break;
		case 32:
			bgPath = "romfs:/gfx/bg_downtownSummer.t3x";
			break;
		case 33:
			bgPath = "romfs:/gfx/bg_downtownFall.t3x";
			break;
		case 34:
			bgPath = "romfs:/gfx/bg_downtownWinter.t3x";
			break;
		case 35:
			bgPath = "romfs:/gfx/bg_cafe2Spring.t3x";
			break;
		case 36:
			bgPath = "romfs:/gfx/bg_cafe2Summer.t3x";
			break;
		case 37:
			bgPath = "romfs:/gfx/bg_cafe2Fall.t3x";
			break;
		case 38:
			bgPath = "romfs:/gfx/bg_cafe2Winter.t3x";
			break;
		case 39:
			if (hour >= 7 && hour < 19) {
				bgPath = "romfs:/gfx/bgDay_exhibitionHall2.t3x";
			} else {
				bgPath = "romfs:/gfx/bgNight_exhibitionHall2.t3x";
				timeOutside = 2;
			}
			break;
		case 40:
			bgPath = "romfs:/gfx/bg_beautician2.t3x";
			break;
		case 41:
			bgPath = "romfs:/gfx/bg_afterParty.t3x";
			break;
		case 42:
			bgPath = "romfs:/gfx/bg_graceShop.t3x";
			break;
		case 43:
			bgPath = "romfs:/gfx/bg_hairSalon1.t3x";
			break;
		case 44:
			bgPath = "romfs:/gfx/bg_beautician1.t3x";
			break;
	}
	bgSprite[0]		= C2D_SpriteSheetLoad(bgPath);
	doBgSpriteFree[0] = true;
	doBgSpriteFree[1] = false;
	doBgSpriteFree[2] = false;
	doBgSpriteFree[3] = false;
	bgAnimationFrame = 0;
	bgAnimationCurrent = 0;
	bgAnimationTime = 0;
	animateBg = false;

	// Load animated parts
	if (studioBg == 12 && timeOutside == 0) {
		bgSprite[1]	= C2D_SpriteSheetLoad("romfs:/gfx/bgDay_tropicaBeach_1.t3x");
		bgSprite[2]	= C2D_SpriteSheetLoad("romfs:/gfx/bgDay_tropicaBeach_2.t3x");
		doBgSpriteFree[1] = true;
		doBgSpriteFree[2] = true;
		//bgAnimationDelay = iFps;
		bgAnimation[0] = 0;
		bgAnimation[1] = 1;
		bgAnimation[2] = 2;
		bgAnimation[3] = 1;
		bgAnimation[4] = 100;
		animateBg = true;
	}
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

	C2D_Image image = C2D_SpriteSheetGetImage(bgSprite[bgAnimationFrame], 0);
	if (!gfxIsWide()) {
		C3D_TexSetFilter(image.tex, GPU_LINEAR, GPU_LINEAR);
	}

	C2D_DrawImageAt(image, 0, yPos-(shiftBySubPixel ? 0.5f : 0), 0.5f, NULL, 0.5, 1);

	if (animateBg) {
		// Animate background
		bgAnimationTime++;
		if (bgAnimationTime >= iFps) {
			bgAnimationCurrent++;
			if (bgAnimation[bgAnimationCurrent] == 100) {
				// Reset animation
				bgAnimationCurrent = 0;
			}
			bgAnimationFrame = bgAnimation[bgAnimationCurrent];
			bgAnimationTime = 0;
		}
	}
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
		if (lightingEffects) {
			switch (studioBg) {
				default:
					break;
				case 7:
					C2D_PlainImageTint(&tint, C2D_Color32(0, 0, 95, 255), 0.1);	// Tint for Live Music Club
					break;
				case 11:
					C2D_PlainImageTint(&tint, C2D_Color32(191, 63, 87, 255), 0.1);	// Tint for Cinema
					break;
				case 1:
				case 12:
				case 13:
				case 14:
					if (timeOutside == 2) {
						C2D_PlainImageTint(&tint, C2D_Color32(0, 0, 95, 255), 0.1);	// Tint for Nighttime
					}
					break;
			}
		}
		C2D_DrawImageAt(image, (cinemaWide ? 60 : 0), yPos-(shiftBySubPixel ? 0.5f : 0), 0.5f, &tint, (cinemaWide ? 0.35f : 0.5), (cinemaWide ? 0.7f : 1));
	} else {
		C2D_PlainImageTint(&tint, C2D_Color32(255, 255, 255, fadeAlpha), 1);
		C2D_DrawImageAt(image, (cinemaWide ? 60 : 0), yPos-(shiftBySubPixel ? 0.5f : 0), 0.5f, &tint, (cinemaWide ? 0.35f : 0.5), (cinemaWide ? 0.7f : 1));
	}
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