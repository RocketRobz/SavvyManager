#include "gameSelect.hpp"
#include "rocketRobz.hpp"
#include "screenvars.h"

void RocketRobz::Draw(void) const {
	Gui::ScreenDraw(Top);

	GFX::DrawSprite(sprites_logo_rocketrobz_idx, 0, 0);
	Gui::DrawString(8, 218, 0.50, WHITE, this->yeartext);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);
	/*this->text_width = 104;
	Gui::DrawString(((320-this->text_width)/2), 100, 0.50, BLACK, this->yeartext);
	this->text_width = 264;
	Gui::DrawString(((320-this->text_width)/2), 116, 0.50, BLACK, this->yeartext2);*/
	//GFX::DrawSprite(sprites_logo_SSanniversary_idx, 32, 24);
	GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void RocketRobz::Logic(u32 hDown, u32 hHeld, touchPosition touch) { }