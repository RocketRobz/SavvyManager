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

extern bool ss3DLCharactersBackedUp;

CharacterChange::CharacterChange() {
	getList();
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

	getMaxChars();
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

const char* CharacterChange::import_characterPreviewFileName(void) const {
	switch (import_highlightedGame) {
		case 2:
		switch (seasonNo) {
			case 0:
				return ss3CharacterPreviewFileNamesSpring[importCharacterList_cursorPosition];
			case 1:
				return ss3CharacterPreviewFileNamesSummer[importCharacterList_cursorPosition];
			case 2:
				return ss3CharacterPreviewFileNamesFall[importCharacterList_cursorPosition];
			case 3:
				return ss3CharacterPreviewFileNamesWinter[importCharacterList_cursorPosition];
		}
		case 3:
		switch (seasonNo) {
			case 0:
				return ss4CharacterPreviewFileNamesSpring[importCharacterList_cursorPosition];
			case 1:
				return ss4CharacterPreviewFileNamesSummer[importCharacterList_cursorPosition];
			case 2:
				return ss4CharacterPreviewFileNamesFall[importCharacterList_cursorPosition];
			case 3:
				return ss4CharacterPreviewFileNamesWinter[importCharacterList_cursorPosition];
		}
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
	if (messageNo == 6) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Characters from the 1st, 2nd, and");
		Gui::DrawStringCentered(0, 78, 0.60, BLACK, "4th games, will leave from the 3rd.");
		Gui::DrawStringCentered(0, 104, 0.60, BLACK, "Characters part of downloaded");
		Gui::DrawStringCentered(0, 124, 0.60, BLACK, "Caprice Chalet rooms (if exists)");
		Gui::DrawStringCentered(0, 144, 0.60, BLACK, "will be restored. Is this OK?");
	} else if (messageNo == 5) {
		Gui::DrawStringCentered(0, 68, 0.60, BLACK, "Everyone is now in Fashion Forward!");
		Gui::DrawStringCentered(0, 88, 0.60, BLACK, "(Except for customers and reps.)");
		Gui::DrawStringCentered(0, 114, 0.60, BLACK, "Invite them over for photo shoots,");
		Gui::DrawStringCentered(0, 134, 0.60, BLACK, "as well as AR photo shoots!");
	} else if (messageNo == 4) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Characters from the 1st, 2nd, and");
		Gui::DrawStringCentered(0, 78, 0.60, BLACK, "4th games, will be added to the 3rd.");
		Gui::DrawStringCentered(0, 104, 0.60, BLACK, "Characters part of downloaded");
		Gui::DrawStringCentered(0, 124, 0.60, BLACK, "Caprice Chalet rooms will be");
		Gui::DrawStringCentered(0, 144, 0.60, BLACK, "backed-up. Is this OK?");
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
	if (messageNo == 4 || messageNo == 6) {
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
	previewCharacter = false;
	gspWaitForVBlank();
	if (import_highlightedGame == 4) {
		if (numberOfExportedCharacters > 0) {
			sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS%i/characters/previews/%s.t3x", highlightedGame+1, getExportedCharacterName(importCharacterList_cursorPosition));	// All Seasons
		} else {
			sprintf(chrFilePath, "romfs:/gfx/null.t3x");	// All Seasons
		}
		previewCharacterFound = GFX::loadCharSprite(chrFilePath, chrFilePath);
	} else if (import_highlightedGame >= 2) {
		sprintf(chrFilePath, "romfs:/gfx/ss%i_%s.t3x", highlightedGame+1, import_characterPreviewFileName());
		previewCharacterFound = GFX::loadCharSprite(chrFilePath, chrFilePath);
	} else {
		sprintf(chrFilePath, "romfs:/gfx/ss%i_%s.t3x", highlightedGame+1, (Robz ? "Robz" : import_characterName()));				// All Seasons
		sprintf(chrFilePath2, "romfs:/gfx/ss%i_%s%i.t3x", highlightedGame+1, (Robz ? "Robz" : import_characterName()), seasonNo);	// One Season
		previewCharacterFound = GFX::loadCharSprite(chrFilePath, chrFilePath2);
	}
	previewCharacter = true;
}

void CharacterChange::addEveryone(void) {
//	if (highlightedGame != 2) return;

	backupSS3DLCharacters("sdmc:/3ds/SavvyManager/SS3/dlCharacters.bak");

	for (int i = 0; i < 49; i++) {
		sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_everyCharacterNames[i]);
		if (access(chrFilePath, F_OK) != 0) {
			sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), import_everyCharacterNames[i]);
		}
		readSS3CharacterFile(0x0BB9+i, chrFilePath);
		if (removeBags) {
			removeSS3CharacterBag(0x0BB9+i);
		}
		sprintf(chrFilePath, "romfs:/character/Fashion Forward/Profiles/%s.cprf", sysRegion==CFG_REGION_EUR||sysRegion==CFG_REGION_AUS ? import_everyCharacterProfileNamesEUR[i] : import_everyCharacterNames[i]);
		readSS3ProfileFile(0x0BB9+i, chrFilePath);
		toggleSS3Character(0x0BB9+i, true);
	}
	writeSS3Save();
	ss3DLCharactersBackedUp = true;
}

void CharacterChange::removeEveryone(void) {
//	if (highlightedGame != 2) return;

	restoreSS3DLCharacters("sdmc:/3ds/SavvyManager/SS3/dlCharacters.bak");
	writeSS3Save();
	ss3DLCharactersBackedUp = false;

	sprintf(chararacterImported, "Characters removed successfully.");
	for (u16 id = 0x0BB9; id <= 0x0BFD; id++) {
		if (existsSS3Character(id)) {
			sprintf(chararacterImported, "Character(s) restored successfully.");
			break;
		}
	}
}


void CharacterChange::Draw(void) const {
	GFX::unloadGameSelSheets();

	Gui::ScreenDraw(Top);

	//GFX::showBgSprite(zoomIn);
	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	if (previewCharacter) {
		if (previewCharacterFound) {
			GFX::showCharSprite(zoomIn, charFadeAlpha);
		} else {
			Gui::DrawStringCentered(0, 104, 0.65, BLACK, (import_highlightedGame==4 ? "Preview not found." : "Preview unavailable."));
		}
	}

	preview();

	if (showMessage && messageNo == 4) {
		Gui::DrawString(8, (cinemaWide ? 174 : 210), 0.50, BLACK, removeBags ? " Keep bags" : " Remove bags");
		// Selected season
		Gui::DrawString(160, (cinemaWide ? 174 : 210), 0.65, BLACK, "L");
		Gui::DrawStringCentered(0, (cinemaWide ? 174 : 210), 0.50, BLACK, seasonName());
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

	cursorX = 248;
	if (subScreenMode == 4) {
		cursorY = 64+(48*importCharacterList_cursorPositionOnScreen);

		// Game name
		switch (import_highlightedGame) {
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
			Gui::DrawStringCentered(0, 210, 0.50, BLACK, seasonName());
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
		cursorY = 64+(48*characterChangeMenu_cursorPositionOnScreen);

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
		cursorY = 64+(48*characterList_cursorPositionOnScreen);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the character you want to change.");

		if (highlightedGame == 2) {
			Gui::DrawString(116, 210, 0.50, BLACK, ss3DLCharactersBackedUp ? "START: Remove contacts" : "START: Expand contacts");
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

	GFX::drawCursor(cursorX, cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}

void CharacterChange::preview() const {
	if (previewCharacter) {
		switch (iFps) {
			default:
				charFadeAlpha += 20;
				break;
			case 30:
				charFadeAlpha += 40;
				break;
			case 24:
				charFadeAlpha += 55;
				break;
		}
		if (charFadeAlpha > 255) charFadeAlpha = 255;
	} else {
		charFadeAlpha = 0;
	}
}



void CharacterChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_CPAD_UP) {
		zoomIn++;
		if (zoomIn > 2) zoomIn = 2;
	}
	
	if (hDown & KEY_CPAD_DOWN) {
		zoomIn--;
		if (zoomIn < 0) zoomIn = 0;
	}

	if (showMessage) {
		if (messageNo == 6) {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 176 && touch.px < 176+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				removeEveryone();
				messageNo = 1;
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndBack();
				showMessage = false;
			}
		} else if (messageNo == 4) {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 176 && touch.px < 176+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				addEveryone();
				messageNo = 5;
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndBack();
				showMessage = false;
			}

			if (hDown & KEY_Y) {
				sndHighlight();
				removeBags = !removeBags;
			}

			if ((hDown & KEY_L) || (hDown & KEY_ZL)) {
				sndHighlight();
				seasonNo--;
				if (seasonNo < 0) seasonNo = 3;
			}

			if ((hDown & KEY_R) || (hDown & KEY_ZR)) {
				sndHighlight();
				seasonNo++;
				if (seasonNo > 3) seasonNo = 0;
			}
		} else {
			if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndSelect();
				if (messageNo == 5) {
					messageNo = 1;
					sprintf(chararacterImported, "Characters imported successfully.");
				} else {
					if (subScreenMode == 1) {
						previewCharacter = false;
					}
					showMessage = false;
				}
			}
		}
	} else if (subScreenMode == 4) {
		bool robzAction = false;
		if (hDown) {
			cheatKeys[cheatKeyPosition] = hDown;
			cheatKeyPosition++;
		}
		robzAction =  ((cheatKeys[0] & KEY_DUP)
					&& (cheatKeys[1] & KEY_DUP)
					&& (cheatKeys[2] & KEY_DDOWN)
					&& (cheatKeys[3] & KEY_DDOWN)
					&& (cheatKeys[4] & KEY_DLEFT)
					&& (cheatKeys[5] & KEY_DRIGHT)
					&& (cheatKeys[6] & KEY_DLEFT)
					&& (cheatKeys[7] & KEY_DRIGHT)
					&& (cheatKeys[8] & KEY_B)
					&& (cheatKeys[9] & KEY_A));
		if (cheatKeyPosition==10
		|| ((cheatKeys[0] != 0) && !(cheatKeys[0] & KEY_DUP))
		|| ((cheatKeys[1] != 0) && !(cheatKeys[1] & KEY_DUP))
		|| ((cheatKeys[2] != 0) && !(cheatKeys[2] & KEY_DDOWN))
		|| ((cheatKeys[3] != 0) && !(cheatKeys[3] & KEY_DDOWN))
		|| ((cheatKeys[4] != 0) && !(cheatKeys[4] & KEY_DLEFT))
		|| ((cheatKeys[5] != 0) && !(cheatKeys[5] & KEY_DRIGHT))
		|| ((cheatKeys[6] != 0) && !(cheatKeys[6] & KEY_DLEFT))
		|| ((cheatKeys[7] != 0) && !(cheatKeys[7] & KEY_DRIGHT))
		|| ((cheatKeys[8] != 0) && !(cheatKeys[8] & KEY_B))
		|| ((cheatKeys[9] != 0) && !(cheatKeys[9] & KEY_A))) {
			for (int i= 0; i < 10; i++) {
				cheatKeys[i] = 0;
			}
			cheatKeyPosition = 0;
		}
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				importCharacterList_cursorPosition--;
				importCharacterList_cursorPositionOnScreen--;
				if (importCharacterList_cursorPosition < 0) {
					importCharacterList_cursorPosition = 0;
					import_characterShownFirst = 0;
				} else if (importCharacterList_cursorPosition < import_characterShownFirst) {
					import_characterShownFirst--;
				}
				if (importCharacterList_cursorPositionOnScreen < 0) {
					importCharacterList_cursorPositionOnScreen = 0;
				}
				loadChrImage(false);
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				importCharacterList_cursorPosition++;
				importCharacterList_cursorPositionOnScreen++;
				if (importCharacterList_cursorPosition > import_totalCharacters) {
					importCharacterList_cursorPosition = import_totalCharacters;
					import_characterShownFirst = import_totalCharacters-2;
					if (import_characterShownFirst < 0) import_characterShownFirst = 0;
					if (importCharacterList_cursorPositionOnScreen > import_totalCharacters) {
						importCharacterList_cursorPositionOnScreen = import_totalCharacters;
					}
				} else if (importCharacterList_cursorPosition > import_characterShownFirst+2) {
					import_characterShownFirst++;
				}
				if (importCharacterList_cursorPositionOnScreen > 2) {
					importCharacterList_cursorPositionOnScreen = 2;
				}
				loadChrImage(false);
			}
		}

		if (hDown & KEY_A) {
			if (robzAction) {
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", "Robz");
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", seasonName(), "Robz");
						}
						readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS4Save();
						break;
					case 2:
						sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", "Robz");
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), "Robz");
						}
						readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS3Save();
						break;
					case 1:
						sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", "Robz");
						if (access(chrFilePath, F_OK) != 0) {
						sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), "Robz");
						}
						readSS2CharacterFile(chrFilePath);
						writeSS2Save();
						break;
				}
				loadChrImage(true);
				sprintf(chararacterImported, "Imported %s successfully.", "Robz");
				messageNo = 1;
				subScreenMode = 1;
				showMessage = true;
			} else if (import_highlightedGame == 4 && numberOfExportedCharacters > 0) {
				bool exportFound = false;
				switch (highlightedGame) {
					case 3:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
							writeSS4Save();
							exportFound = true;
						}
						break;
					case 2:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
							writeSS3Save();
							exportFound = true;
						}
						break;
					case 1:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS2CharacterFile(chrFilePath);
							writeSS2Save();
							exportFound = true;
						}
						break;
				}
				if (exportFound) {
					sprintf(chararacterImported, "Imported %s successfully.", getExportedCharacterName(importCharacterList_cursorPosition));
					messageNo = 1;
					subScreenMode = 1;
				} else {
					sndBack();
					messageNo = 3;
				}
				showMessage = true;
			} else if (import_highlightedGame != 4) {
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS4Save();
						break;
					case 2:
						sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS3Save();
						break;
					case 1:
						sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS2CharacterFile(chrFilePath);
						writeSS2Save();
						break;
				}
				sprintf(chararacterImported, "Imported %s successfully.", import_characterNameDisplay());
				messageNo = 1;
				subScreenMode = 1;
				showMessage = true;
				}
			}

		if (hDown & KEY_DLEFT) {
			sndHighlight();
			import_highlightedGame--;
			if (import_highlightedGame < 0) import_highlightedGame = 4;
			getMaxChars();
		}

		if (hDown & KEY_DRIGHT) {
			sndHighlight();
			import_highlightedGame++;
			if (import_highlightedGame > 4) import_highlightedGame = 0;
			getMaxChars();
		}

		if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
			importCharacterList_cursorPosition = 0;
			importCharacterList_cursorPositionOnScreen = 0;
			import_characterShownFirst = 0;
			if (import_highlightedGame == 4) {
				previewCharacter = false;
				if (!exportedCharListGotten[highlightedGame]) {
					displayNothing = true;
					gspWaitForVBlank();
					getExportedCharacterContents();
					exportedCharListGotten[highlightedGame] = true;
					displayNothing = false;
				}
			}
			getMaxChars();
			loadChrImage(false);
		}

		if (import_highlightedGame != 4) {
			if ((hDown & KEY_L) || (hDown & KEY_ZL)) {
				sndHighlight();
				seasonNo--;
				if (seasonNo < 0) seasonNo = 3;
				loadChrImage(false);
			}

			if ((hDown & KEY_R) || (hDown & KEY_ZR)) {
				sndHighlight();
				seasonNo++;
				if (seasonNo > 3) seasonNo = 0;
				loadChrImage(false);
			}
		}

		if (((hDown & KEY_B) && !(cheatKeys[8] & KEY_B))
		|| ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 1;
			previewCharacter = false;
		}

	} else if (subScreenMode == 1) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				characterChangeMenu_cursorPosition--;
				characterChangeMenu_cursorPositionOnScreen--;
				if (characterChangeMenu_cursorPosition < 0) {
					characterChangeMenu_cursorPosition = 0;
					characterChangeMenu_optionShownFirst = 0;
				} else if (characterChangeMenu_cursorPosition < characterChangeMenu_optionShownFirst) {
					characterChangeMenu_optionShownFirst--;
				}
				if (characterChangeMenu_cursorPositionOnScreen < 0) {
					characterChangeMenu_cursorPositionOnScreen = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				characterChangeMenu_cursorPosition++;
				characterChangeMenu_cursorPositionOnScreen++;
				if (characterChangeMenu_cursorPosition > characterChangeMenuOptions) {
					characterChangeMenu_cursorPosition = characterChangeMenuOptions;
					characterChangeMenu_optionShownFirst = characterChangeMenuOptions-2;
				} else if (characterChangeMenu_cursorPosition > characterChangeMenu_optionShownFirst+2) {
					characterChangeMenu_optionShownFirst++;
				}

				if (characterChangeMenu_cursorPositionOnScreen > 2) {
					characterChangeMenu_cursorPositionOnScreen = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			if (characterChangeMenuOps[characterChangeMenu_cursorPosition] == 0) {
				sndBack();
				messageNo = 0;
				showMessage = true;
			} else if (characterChangeMenuOps[characterChangeMenu_cursorPosition] == 10) {
				// Export character
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", characterName(false));
						writeSS4CharacterFile(characterList_cursorPosition, chrFilePath);
						break;
					case 2:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", characterName(false));
						writeSS3CharacterFile(characterList_cursorPosition, chrFilePath);
						break;
					case 1:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", characterName(false));
						writeSS2CharacterFile(chrFilePath);
						break;
				}
				exportedCharListGotten[highlightedGame] = false;
				messageNo = 2;
				showMessage = true;
			} else {
				sndSelect();
				displayNothing = true;
				subScreenMode = characterChangeMenuOps[characterChangeMenu_cursorPosition];
				if ((subScreenMode == 4) && (import_highlightedGame == 4) && !exportedCharListGotten[highlightedGame]) {
					gspWaitForVBlank();
					getExportedCharacterContents();
					exportedCharListGotten[highlightedGame] = true;
				}
				displayNothing = false;
				loadChrImage(false);
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 0;
		}
	
	} else {
		if (showCursor) {
			if ((hDown & KEY_DUP) && (highlightedGame > 1)) {
				sndHighlight();
				characterList_cursorPosition--;
				characterList_cursorPositionOnScreen--;
				if (characterList_cursorPosition < 0) {
					characterList_cursorPosition = 0;
					characterShownFirst = 0;
				} else if (characterList_cursorPosition < characterShownFirst) {
					characterShownFirst--;
				}
				if (characterList_cursorPositionOnScreen < 0) {
					characterList_cursorPositionOnScreen = 0;
				}
			}

			if ((hDown & KEY_DDOWN) && (highlightedGame > 1)) {
				sndHighlight();
				characterList_cursorPosition++;
				characterList_cursorPositionOnScreen++;
				if (characterList_cursorPosition > totalCharacters) {
					characterList_cursorPosition = totalCharacters;
					characterShownFirst = totalCharacters-2;
				} else if (characterList_cursorPosition > characterShownFirst+2) {
					characterShownFirst++;
				}
				if (characterList_cursorPositionOnScreen > 2) {
					characterList_cursorPositionOnScreen = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			characterList_cursorPosition = 0;
			characterList_cursorPositionOnScreen = 0;
			characterShownFirst = 0;
			characterChangeMenu_cursorPosition = 0;
			characterChangeMenu_cursorPositionOnScreen = 0;
			characterChangeMenu_optionShownFirst = 0;
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}

		if ((hDown & KEY_START) && (highlightedGame == 2)) {
			sndSelect();
			messageNo = ss3DLCharactersBackedUp ? 6 : 4;
			showMessage = true;
		}
	}
}