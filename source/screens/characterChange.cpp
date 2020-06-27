#include "characterChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "savedata.h"
#include "file_browse.h"

#include "ss3charnames.h"
#include "ss4charnames.h"
#include "import_ss1charnames.h"
#include "import_ss2charnames.h"
#include "import_ss3charnames.h"
#include "import_ss4charnames.h"
#include "import_everycharnames.h"

#include <unistd.h>

CharacterChange::CharacterChange() {
	this->getList();
}

void CharacterChange::getList() {
	if (highlightedGame == 3) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 0;
		characterChangeMenuOps[2] = 4;
		characterChangeMenuOps[3] = 10;
		characterChangeMenuOptions = 3;
		totalCharacters = 0x20;
		readSS4Save();
	} else if (highlightedGame == 2) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 4;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0xE;
		readSS3Save();
	} else if (highlightedGame == 1) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 4;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0;
		readSS2Save();
	}

	this->getMaxChars();
}

void CharacterChange::getMaxChars() {
	if (import_highlightedGame == 4) {
		import_totalCharacters = numberOfExportedCharacters-1;
	} else if (import_highlightedGame == 3) {
		import_totalCharacters = 0xD;
	} else if (import_highlightedGame == 2) {
		import_totalCharacters = 0x10;
	} else if (import_highlightedGame == 1) {
		import_totalCharacters = 0x12;
	} else if (import_highlightedGame == 0) {
		import_totalCharacters = 0x7;
	}
}

const char* CharacterChange::seasonName(void) const {
	switch (seasonNo) {
		case 0:
			return "Spring";
		case 1:
			return "Summer";
		case 2:
			return "Fall";
		case 3:
			return "Winter";
	}
	
	return "null";
}

const char* CharacterChange::characterName(bool showPlayerName) const {
	if (characterList_cursorPosition == 0) {
		if (showPlayerName) {
			switch (highlightedGame) {
				case 1:
					return ss2PlayerName;
				case 2:
					return ss3PlayerName;
				case 3:
					return ss4PlayerName;
			}
		} else {
			return "Player";
		}
	}

	switch (highlightedGame) {
		case 2:
			return ss3CharacterNames[characterList_cursorPosition];
		case 3:
			return ss4CharacterNames[characterList_cursorPosition];
	}
	return "null";
}

const char* CharacterChange::import_characterName(void) const {
	switch (import_highlightedGame) {
		case 0:
			return import_ss1CharacterNames[importCharacterList_cursorPosition];
		case 1:
			return import_ss2CharacterNames[importCharacterList_cursorPosition];
		case 2:
			return import_ss3CharacterNames[importCharacterList_cursorPosition];
		case 3:
			return import_ss4CharacterNames[importCharacterList_cursorPosition];
	}
	return "null";
}

const char* CharacterChange::import_SS2CharacterNames(int i) const {
	switch (sysRegion) {
		default:
			return import_ss2CharacterNames[i];
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return import_nsbCharacterNames[i];
	}
}

const char* CharacterChange::import_characterNameDisplay(void) const {
	switch (import_highlightedGame) {
		case 0:
			return import_ss1CharacterNames[importCharacterList_cursorPosition];
		case 1:
			return import_SS2CharacterNames(importCharacterList_cursorPosition);
		case 2:
			return import_ss3CharacterNames[importCharacterList_cursorPosition];
		case 3:
			return import_ss4CharacterNames[importCharacterList_cursorPosition];
	}
	return "null";
}

const char* CharacterChange::ss1Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "Style Boutique";
		case CFG_REGION_JPN:
			return "Wagamama Fashion: Girls Mode";
		case CFG_REGION_KOR:
			return "Namanui Collection: Girls Style";
	}
}

const char* CharacterChange::ss2Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Trendsetters";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique";
		case CFG_REGION_JPN:
			return "Girls Mode: Yokubari Sengen";
		case CFG_REGION_KOR:
			return "Girls Style: Paesyeon Lideo Seon-eon!";
	}
}

const char* CharacterChange::ss3Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Fashion Forward";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique 2: Fashion Forward";
		case CFG_REGION_JPN:
			return "Girls Mode 3: Kirakira * Code";
		case CFG_REGION_KOR:
			return "Girls Style: Kirakira * Code";
	}
}

const char* CharacterChange::ss4Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Styling Star";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique 3: Styling Star";
		case CFG_REGION_JPN:
			return "Girls Mode 4: Star Stylist";
		case CFG_REGION_KOR:
			return "Girls Style: Star Stylist";
	}
}

void CharacterChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(messageNo == 4 ? sprites_icon_question_idx : sprites_icon_msg_idx, 132, -2);
	if (messageNo == 5) {
		Gui::DrawStringCentered(0, 68, 0.60, BLACK, "Everyone is now in Fashion Forward!");
		Gui::DrawStringCentered(0, 88, 0.60, BLACK, "(Except for customers and reps.)");
		Gui::DrawStringCentered(0, 114, 0.60, BLACK, "Invite them over for photo shoots,");
		Gui::DrawStringCentered(0, 134, 0.60, BLACK, "as well as AR photo shoots!");
	} else if (messageNo == 4) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Characters from the 1st, 2nd, and");
		Gui::DrawStringCentered(0, 78, 0.60, BLACK, "4th games, will be added to the 3rd.");
		Gui::DrawStringCentered(0, 104, 0.60, BLACK, "Characters part of downloaded");
		Gui::DrawStringCentered(0, 124, 0.60, BLACK, "Caprice Chalet rooms will be");
		Gui::DrawStringCentered(0, 144, 0.60, BLACK, "overwritten. Is this OK?");
	} else if (messageNo == 3) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Failed to import character.");
	} else if (messageNo == 2) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Character exported successfully.");
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "You can go to \"Import Characters\"");
		Gui::DrawStringCentered(0, 114, 0.60, BLACK, "and restore the exported character");
		Gui::DrawStringCentered(0, 134, 0.60, BLACK, "at any time.");
	} else if (messageNo == 1) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, chararacterImported);
	} else {
		Gui::DrawStringCentered(0, 84, 0.60, BLACK, "This feature is not available yet.");
		//Gui::DrawStringCentered(0, 104, 0.60, BLACK, "yet.");
	}
	if (messageNo == 4) {
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 52, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 53, 188);
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 176, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 177, 188);
		Gui::DrawString(72, 196, 0.70, MSG_BUTTONTEXT, " No");
		Gui::DrawString(196, 196, 0.70, MSG_BUTTONTEXT, " Yes");
	} else {
		GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
		GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
		Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
	}
}

void CharacterChange::loadChrImage(bool Robz) {
	this->previewCharacter = false;
	gspWaitForVBlank();
	if (import_highlightedGame == 4) {
		if (numberOfExportedCharacters > 0) {
			sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS%i/characters/previews/%s.t3x", highlightedGame+1, getExportedCharacterName(this->importCharacterList_cursorPosition));	// All Seasons
		} else {
			sprintf(this->chrFilePath, "romfs:/gfx/null.t3x");	// All Seasons
		}
		this->previewCharacterFound = GFX::loadCharSprite(this->chrFilePath, this->chrFilePath);
	} else {
		sprintf(this->chrFilePath, "romfs:/gfx/ss%i_%s.t3x", highlightedGame+1, (Robz ? "Robz" : import_characterName()));				// All Seasons
		sprintf(this->chrFilePath2, "romfs:/gfx/ss%i_%s%i.t3x", highlightedGame+1, (Robz ? "Robz" : import_characterName()), this->seasonNo);	// One Season
		this->previewCharacterFound = GFX::loadCharSprite(this->chrFilePath, this->chrFilePath2);
	}
	this->previewCharacter = true;
}

void CharacterChange::addEveryone(void) {
//	if (highlightedGame != 2) return;
	
	for (int i = 0; i < 49; i++) {
		sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_everyCharacterNames[i]);
		if (access(this->chrFilePath, F_OK) != 0) {
			sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", this->seasonName(), import_everyCharacterNames[i]);
		}
		readSS3CharacterFile(0x0BB9+i, chrFilePath);
		if (this->removeBags) {
			removeSS3CharacterBag(0x0BB9+i);
		}
		sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/Profiles/%s.cprf", sysRegion==CFG_REGION_EUR||sysRegion==CFG_REGION_AUS ? import_everyCharacterProfileNamesEUR[i] : import_everyCharacterNames[i]);
		readSS3ProfileFile(0x0BB9+i, this->chrFilePath);
		toggleSS3Character(0x0BB9+i, true);
	}
	writeSS3Save();
}


void CharacterChange::Draw(void) const {
	Gui::ScreenDraw(Top);

	//GFX::showBgSprite(zoomIn);
	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	if (this->previewCharacter) {
		if (this->previewCharacterFound) {
			GFX::showCharSprite(this->zoomIn, this->charFadeAlpha);
		} else {
			Gui::DrawStringCentered(0, 104, 0.65, BLACK, (this->import_highlightedGame==4 ? "Preview not found." : "Preview unavailable."));
		}
	}

	this->preview();

	if (this->showMessage && this->messageNo == 4) {
		Gui::DrawString(8, (cinemaWide ? 174 : 210), 0.50, BLACK, this->removeBags ? " Keep bags" : " Remove bags");
		// Selected season
		Gui::DrawString(160, (cinemaWide ? 174 : 210), 0.65, BLACK, "L");
		Gui::DrawStringCentered(0, (cinemaWide ? 174 : 210), 0.50, BLACK, this->seasonName());
		Gui::DrawString(232, (cinemaWide ? 174 : 210), 0.65, BLACK, "R");
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	if (shiftBySubPixel) {
		Gui::Draw_Rect(0, 239, 400, 1, C2D_Color32(0, 0, 0, 255));
		return;
	}
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	this->cursorX = 248;
	if (this->subScreenMode == 4) {
		this->cursorY = 64+(48*this->importCharacterList_cursorPositionOnScreen);

		// Game name
		switch (this->import_highlightedGame) {
			case 4:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, "Your character files");
				break;
			case 3:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, ss4Title());
				break;
			case 2:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, ss3Title());
				break;
			case 1:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, ss2Title());
				break;
			case 0:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, ss1Title());
				break;
		}
		Gui::DrawString(8, 8, 0.50, BLACK, "<");
		Gui::DrawString(304, 8, 0.50, BLACK, ">");

		if (import_highlightedGame != 4) {
			// Selected season
			Gui::DrawString(120, 208, 0.65, BLACK, "L");
			Gui::DrawStringCentered(0, 210, 0.50, BLACK, this->seasonName());
			Gui::DrawString(192, 208, 0.65, BLACK, "R");
		}

	  if (!displayNothing) {
		int i2 = 48;
		for (int i = import_characterShownFirst; i < import_characterShownFirst+3; i++) {
			if (import_highlightedGame == 4) {
				if (i >= numberOfExportedCharacters) break;
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((getExportedCharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, getExportedCharacterName(i));
			} else if (import_highlightedGame == 3) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss4CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, import_ss4CharacterNames[i]);
			} else if (import_highlightedGame == 2) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss3CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, import_ss3CharacterNames[i]);
			} else if (import_highlightedGame == 1) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss2CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx)/*+import_ss2CharacterTieColors[i]*/, 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, import_SS2CharacterNames(i));
			} else if (import_highlightedGame == 0) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss1CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, import_ss1CharacterNames[i]);
			}
			i2 += 48;
		}
	  }
	} else if (subScreenMode == 1) {
		this->cursorY = 64+(48*characterChangeMenu_cursorPositionOnScreen);

		Gui::DrawString(8, 8, 0.50, BLACK, characterName(true));

		int i2 = 0;
		if (characterChangeMenu_optionShownFirst == 0) {
			i2 += 48;
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2, 0.65, BLACK, "Change attributes");
		}
		if (highlightedGame == 3) {
			i2 += 48;
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2, 0.65, BLACK, "Change bow placement");
		}
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Import character");
		if (highlightedGame < 3 || characterChangeMenu_optionShownFirst == 1) {
			i2 += 48;
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2, 0.65, BLACK, "Export character");
		}
	} else {
		this->cursorY = 64+(48*characterList_cursorPositionOnScreen);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the character you want to change.");

		if (highlightedGame == 2) {
			Gui::DrawString(116, 210, 0.50, BLACK, "START: Expand contacts");
		}

	  if (!displayNothing) {
		int i2 = 48;
		for (int i = characterShownFirst; i < characterShownFirst+3; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (highlightedGame == 3) {
				if (i==0) {
					GFX::DrawSprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, BLACK, ss4PlayerName);
				} else {
					GFX::DrawSprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, BLACK, ss4CharacterNames[i]);
				}
			} else if (highlightedGame == 2) {
				if (i==0) {
					GFX::DrawSprite((getSS3CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, BLACK, ss3PlayerName);
				} else {
					GFX::DrawSprite((getSS3CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, BLACK, ss3CharacterNames[i]);
				}
			} else if (highlightedGame == 1) {
				GFX::DrawSprite((getSS2CharacterGender() ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, ss2PlayerName);
				break;
			}
			i2 += 48;
		}
	  }
	}

	GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
	GFX::DrawSprite(sprites_button_red_idx, 5, 195);
	GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
	GFX::DrawSprite(sprites_button_b_idx, 44, 218);

	GFX::drawCursor(this->cursorX, this->cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}

void CharacterChange::preview() const {
	if (this->previewCharacter) {
		switch (iFps) {
			default:
				this->charFadeAlpha += 20;
				break;
			case 30:
				this->charFadeAlpha += 40;
				break;
			case 24:
				this->charFadeAlpha += 55;
				break;
		}
		if (this->charFadeAlpha > 255) this->charFadeAlpha = 255;
	} else {
		this->charFadeAlpha = 0;
	}
}



void CharacterChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_CPAD_UP) {
		this->zoomIn++;
		if (this->zoomIn > 2) this->zoomIn = 2;
	}
	
	if (hDown & KEY_CPAD_DOWN) {
		this->zoomIn--;
		if (this->zoomIn < 0) this->zoomIn = 0;
	}

	if (this->showMessage) {
		if (this->messageNo == 4) {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 176 && touch.px < 176+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				this->addEveryone();
				this->messageNo = 5;
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndBack();
				this->showMessage = false;
			}

			if (hDown & KEY_Y) {
				sndHighlight();
				this->removeBags = !this->removeBags;
			}

			if ((hDown & KEY_L) || (hDown & KEY_ZL)) {
				sndHighlight();
				this->seasonNo--;
				if (this->seasonNo < 0) this->seasonNo = 3;
			}

			if ((hDown & KEY_R) || (hDown & KEY_ZR)) {
				sndHighlight();
				this->seasonNo++;
				if (this->seasonNo > 3) this->seasonNo = 0;
			}
		} else {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				if (this->messageNo == 5) {
					this->messageNo = 1;
					sprintf(this->chararacterImported, "Characters imported successfully.");
				} else {
					if (this->subScreenMode == 1) {
						this->previewCharacter = false;
					}
					this->showMessage = false;
				}
			}
		}
	} else if (this->subScreenMode == 4) {
		bool robzAction = false;
		if (hDown) {
			this->cheatKeys[cheatKeyPosition] = hDown;
			this->cheatKeyPosition++;
		}
		robzAction =  ((this->cheatKeys[0] & KEY_DUP)
					&& (this->cheatKeys[1] & KEY_DUP)
					&& (this->cheatKeys[2] & KEY_DDOWN)
					&& (this->cheatKeys[3] & KEY_DDOWN)
					&& (this->cheatKeys[4] & KEY_DLEFT)
					&& (this->cheatKeys[5] & KEY_DRIGHT)
					&& (this->cheatKeys[6] & KEY_DLEFT)
					&& (this->cheatKeys[7] & KEY_DRIGHT)
					&& (this->cheatKeys[8] & KEY_B)
					&& (this->cheatKeys[9] & KEY_A));
		if (this->cheatKeyPosition==10
		|| ((this->cheatKeys[0] != 0) && !(this->cheatKeys[0] & KEY_DUP))
		|| ((this->cheatKeys[1] != 0) && !(this->cheatKeys[1] & KEY_DUP))
		|| ((this->cheatKeys[2] != 0) && !(this->cheatKeys[2] & KEY_DDOWN))
		|| ((this->cheatKeys[3] != 0) && !(this->cheatKeys[3] & KEY_DDOWN))
		|| ((this->cheatKeys[4] != 0) && !(this->cheatKeys[4] & KEY_DLEFT))
		|| ((this->cheatKeys[5] != 0) && !(this->cheatKeys[5] & KEY_DRIGHT))
		|| ((this->cheatKeys[6] != 0) && !(this->cheatKeys[6] & KEY_DLEFT))
		|| ((this->cheatKeys[7] != 0) && !(this->cheatKeys[7] & KEY_DRIGHT))
		|| ((this->cheatKeys[8] != 0) && !(this->cheatKeys[8] & KEY_B))
		|| ((this->cheatKeys[9] != 0) && !(this->cheatKeys[9] & KEY_A))) {
			for (int i= 0; i < 10; i++) {
				this->cheatKeys[i] = 0;
			}
			this->cheatKeyPosition = 0;
		}
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				this->importCharacterList_cursorPosition--;
				this->importCharacterList_cursorPositionOnScreen--;
				if (this->importCharacterList_cursorPosition < 0) {
					this->importCharacterList_cursorPosition = 0;
					this->import_characterShownFirst = 0;
				} else if (this->importCharacterList_cursorPosition < this->import_characterShownFirst) {
					this->import_characterShownFirst--;
				}
				if (this->importCharacterList_cursorPositionOnScreen < 0) {
					this->importCharacterList_cursorPositionOnScreen = 0;
				}
				this->loadChrImage(false);
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				this->importCharacterList_cursorPosition++;
				this->importCharacterList_cursorPositionOnScreen++;
				if (this->importCharacterList_cursorPosition > this->import_totalCharacters) {
					this->importCharacterList_cursorPosition = this->import_totalCharacters;
					this->import_characterShownFirst = this->import_totalCharacters-2;
					if (this->import_characterShownFirst < 0) this->import_characterShownFirst = 0;
					if (this->importCharacterList_cursorPositionOnScreen > this->import_totalCharacters) {
						this->importCharacterList_cursorPositionOnScreen = this->import_totalCharacters;
					}
				} else if (this->importCharacterList_cursorPosition > this->import_characterShownFirst+2) {
					this->import_characterShownFirst++;
				}
				if (this->importCharacterList_cursorPositionOnScreen > 2) {
					this->importCharacterList_cursorPositionOnScreen = 2;
				}
				this->loadChrImage(false);
			}
		}

		if (hDown & KEY_A) {
			if (robzAction) {
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(this->chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", "Robz");
						if (access(this->chrFilePath, F_OK) != 0) {
							sprintf(this->chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", this->seasonName(), "Robz");
						}
						readSS4CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						writeSS4Save();
						break;
					case 2:
						sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", "Robz");
						if (access(this->chrFilePath, F_OK) != 0) {
							sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", this->seasonName(), "Robz");
						}
						readSS3CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						writeSS3Save();
						break;
					case 1:
						sprintf(this->chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", "Robz");
						if (access(this->chrFilePath, F_OK) != 0) {
						sprintf(this->chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", this->seasonName(), "Robz");
						}
						readSS2CharacterFile(this->chrFilePath);
						writeSS2Save();
						break;
				}
				this->loadChrImage(true);
				sprintf(this->chararacterImported, "Imported %s successfully.", "Robz");
				this->messageNo = 1;
				this->subScreenMode = 1;
				this->showMessage = true;
			} else if (this->import_highlightedGame == 4 && numberOfExportedCharacters > 0) {
				bool exportFound = false;
				switch (highlightedGame) {
					case 3:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS4CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
							writeSS4Save();
							exportFound = true;
						}
						break;
					case 2:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(this->chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS3CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
							writeSS3Save();
							exportFound = true;
						}
						break;
					case 1:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(this->chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS2CharacterFile(this->chrFilePath);
							writeSS2Save();
							exportFound = true;
						}
						break;
				}
				if (exportFound) {
					sprintf(this->chararacterImported, "Imported %s successfully.", getExportedCharacterName(importCharacterList_cursorPosition));
					this->messageNo = 1;
					this->subScreenMode = 1;
				} else {
					sndBack();
					this->messageNo = 3;
				}
				this->showMessage = true;
			} else if (this->import_highlightedGame != 4) {
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(this->chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", import_characterName());
						if (access(this->chrFilePath, F_OK) != 0) {
							sprintf(this->chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", this->seasonName(), import_characterName());
						}
						readSS4CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						writeSS4Save();
						break;
					case 2:
						sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_characterName());
						if (access(this->chrFilePath, F_OK) != 0) {
							sprintf(this->chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", this->seasonName(), import_characterName());
						}
						readSS3CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						writeSS3Save();
						break;
					case 1:
						sprintf(this->chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", import_characterName());
						if (access(this->chrFilePath, F_OK) != 0) {
							sprintf(this->chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", this->seasonName(), import_characterName());
						}
						readSS2CharacterFile(this->chrFilePath);
						writeSS2Save();
						break;
				}
				sprintf(this->chararacterImported, "Imported %s successfully.", import_characterNameDisplay());
				this->messageNo = 1;
				this->subScreenMode = 1;
				this->showMessage = true;
				}
			}

		if (hDown & KEY_DLEFT) {
			sndHighlight();
			this->import_highlightedGame--;
			if (this->import_highlightedGame < 0) this->import_highlightedGame = 4;
			this->getMaxChars();
		}

		if (hDown & KEY_DRIGHT) {
			sndHighlight();
			this->import_highlightedGame++;
			if (this->import_highlightedGame > 4) this->import_highlightedGame = 0;
			this->getMaxChars();
		}

		if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
			this->importCharacterList_cursorPosition = 0;
			this->importCharacterList_cursorPositionOnScreen = 0;
			this->import_characterShownFirst = 0;
			if (this->import_highlightedGame == 4) {
				this->previewCharacter = false;
				if (!this->exportedCharListGotten[highlightedGame]) {
					this->displayNothing = true;
					gspWaitForVBlank();
					getExportedCharacterContents();
					this->exportedCharListGotten[highlightedGame] = true;
					this->displayNothing = false;
				}
			}
			this->getMaxChars();
			this->loadChrImage(false);
		}

		if (this->import_highlightedGame != 4) {
			if ((hDown & KEY_L) || (hDown & KEY_ZL)) {
				sndHighlight();
				this->seasonNo--;
				if (this->seasonNo < 0) this->seasonNo = 3;
				this->loadChrImage(false);
			}

			if ((hDown & KEY_R) || (hDown & KEY_ZR)) {
				sndHighlight();
				this->seasonNo++;
				if (this->seasonNo > 3) this->seasonNo = 0;
				this->loadChrImage(false);
			}
		}

		if (((hDown & KEY_B) && !(this->cheatKeys[8] & KEY_B))
		|| ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			this->subScreenMode = 1;
			this->previewCharacter = false;
		}

	} else if (this->subScreenMode == 1) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				this->characterChangeMenu_cursorPosition--;
				this->characterChangeMenu_cursorPositionOnScreen--;
				if (this->characterChangeMenu_cursorPosition < 0) {
					this->characterChangeMenu_cursorPosition = 0;
					this->characterChangeMenu_optionShownFirst = 0;
				} else if (this->characterChangeMenu_cursorPosition < this->characterChangeMenu_optionShownFirst) {
					this->characterChangeMenu_optionShownFirst--;
				}
				if (this->characterChangeMenu_cursorPositionOnScreen < 0) {
					this->characterChangeMenu_cursorPositionOnScreen = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				this->characterChangeMenu_cursorPosition++;
				this->characterChangeMenu_cursorPositionOnScreen++;
				if (this->characterChangeMenu_cursorPosition > this->characterChangeMenuOptions) {
					this->characterChangeMenu_cursorPosition = this->characterChangeMenuOptions;
					this->characterChangeMenu_optionShownFirst = this->characterChangeMenuOptions-2;
				} else if (this->characterChangeMenu_cursorPosition > this->characterChangeMenu_optionShownFirst+2) {
					this->characterChangeMenu_optionShownFirst++;
				}

				if (this->characterChangeMenu_cursorPositionOnScreen > 2) {
					this->characterChangeMenu_cursorPositionOnScreen = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			if (this->characterChangeMenuOps[characterChangeMenu_cursorPosition] == 0) {
				sndBack();
				this->messageNo = 0;
				this->showMessage = true;
			} else if (this->characterChangeMenuOps[characterChangeMenu_cursorPosition] == 10) {
				// Export character
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", this->characterName(false));
						writeSS4CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						break;
					case 2:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", characterName(false));
						writeSS3CharacterFile(this->characterList_cursorPosition, this->chrFilePath);
						break;
					case 1:
						sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", characterName(false));
						writeSS2CharacterFile(this->chrFilePath);
						break;
				}
				this->exportedCharListGotten[highlightedGame] = false;
				this->messageNo = 2;
				this->showMessage = true;
			} else {
				sndSelect();
				this->displayNothing = true;
				this->subScreenMode = this->characterChangeMenuOps[characterChangeMenu_cursorPosition];
				if ((this->subScreenMode == 4) && (this->import_highlightedGame == 4) && !this->exportedCharListGotten[highlightedGame]) {
					gspWaitForVBlank();
					getExportedCharacterContents();
					this->exportedCharListGotten[highlightedGame] = true;
				}
				this->displayNothing = false;
				this->loadChrImage(false);
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			this->subScreenMode = 0;
		}
	
	} else {
		if (showCursor) {
			if ((hDown & KEY_DUP) && (highlightedGame > 1)) {
				sndHighlight();
				this->characterList_cursorPosition--;
				this->characterList_cursorPositionOnScreen--;
				if (this->characterList_cursorPosition < 0) {
					this->characterList_cursorPosition = 0;
					this->characterShownFirst = 0;
				} else if (this->characterList_cursorPosition < this->characterShownFirst) {
					this->characterShownFirst--;
				}
				if (this->characterList_cursorPositionOnScreen < 0) {
					this->characterList_cursorPositionOnScreen = 0;
				}
			}

			if ((hDown & KEY_DDOWN) && (highlightedGame > 1)) {
				sndHighlight();
				this->characterList_cursorPosition++;
				this->characterList_cursorPositionOnScreen++;
				if (this->characterList_cursorPosition > this->totalCharacters) {
					this->characterList_cursorPosition = this->totalCharacters;
					this->characterShownFirst = this->totalCharacters-2;
				} else if (this->characterList_cursorPosition > this->characterShownFirst+2) {
					this->characterShownFirst++;
				}
				if (this->characterList_cursorPositionOnScreen > 2) {
					this->characterList_cursorPositionOnScreen = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			this->subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			this->characterList_cursorPosition = 0;
			this->characterList_cursorPositionOnScreen = 0;
			this->characterShownFirst = 0;
			this->characterChangeMenu_cursorPosition = 0;
			this->characterChangeMenu_cursorPositionOnScreen = 0;
			this->characterChangeMenu_optionShownFirst = 0;
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}

		if ((hDown & KEY_START) && (highlightedGame == 2)) {
			sndSelect();
			this->messageNo = 4;
			this->showMessage = true;
		}
	}
}