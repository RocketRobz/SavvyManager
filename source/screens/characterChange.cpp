#include "characterChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "commonLut.hpp"
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

extern bool ss2SaveFound;
extern bool ss3SaveFound;
extern bool ss4SaveFound;

const char* getSS3CharName(u16 charId) {
	if (charId >= 0x0BB9 && charId < 0x0BB9+50) {
		return readSS3ProfileName(charId);
	} else if (charId == 0x07D1) {
		return ss3PlayerName;
	} else if (charId >= 0x044C && charId <= 0x0469) {
		return ss3CharacterNamesMaleP3[charId-0x044C];
	} else if (charId >= 0x041A && charId <= 0x0437) {
		return ss3CharacterNamesMaleP2[charId-0x041A];
	} else if (charId >= 0x03E8 && charId <= 0x0405) {
		return ss3CharacterNamesMale[charId-0x03E8];
	} else if (charId >= 0x0352 && charId <= 0x036F) {
		return ss3CharacterNames5S[charId-0x0352];
	} else if (charId >= 0x0320 && charId <= 0x033D) {
		return ss3CharacterNamesReps[charId-0x0320];
	} else if (charId >= 0x02EE && charId <= 0x030B) {
		return ss3CharacterNamesP16[charId-0x02EE];
	} else if (charId >= 0x02BC && charId <= 0x02D9) {
		return ss3CharacterNamesP15[charId-0x02BC];
	} else if (charId >= 0x028A && charId <= 0x02A7) {
		return ss3CharacterNamesP14[charId-0x028A];
	} else if (charId >= 0x0258 && charId <= 0x0275) {
		return ss3CharacterNamesP13[charId-0x0258];
	} else if (charId >= 0x0226 && charId <= 0x0243) {
		return ss3CharacterNamesP12[charId-0x0226];
	} else if (charId >= 0x01F4 && charId <= 0x0211) {
		return ss3CharacterNamesP11[charId-0x01F4];
	} else if (charId >= 0x01C2 && charId <= 0x01DF) {
		return ss3CharacterNamesP10[charId-0x01C2];
	} else if (charId >= 0x0190 && charId <= 0x01AD) {
		return ss3CharacterNamesP9[charId-0x0190];
	} else if (charId >= 0x015E && charId <= 0x017B) {
		return ss3CharacterNamesP8[charId-0x015E];
	} else if (charId >= 0x012C && charId <= 0x0149) {
		return ss3CharacterNamesP7[charId-0x012C];
	} else if (charId >= 0x00FA && charId <= 0x0117) {
		return ss3CharacterNamesP6[charId-0x00FA];
	} else if (charId >= 0x00C8 && charId <= 0xE5) {
		return ss3CharacterNamesP5[charId-0x00C8];
	} else if (charId >= 0x0096 && charId <= 0xB3) {
		return ss3CharacterNamesP4[charId-0x0096];
	} else if (charId >= 0x0064 && charId <= 0x81) {
		return ss3CharacterNamesP3[charId-0x0064];
	} else if (charId >= 0x0032 && charId <= 0x5E) {
		return ss3CharacterNamesP2[charId-0x0032];
	} else if (charId <= 0x001D) {
		return ss3CharacterNames[charId];
	}
	return "???";
}

const char* getSS4CharName(u16 charId) {
	if (charId >= 0x0BB9 && charId < 0x0BB9+30) {
		return readSS4ProfileName(charId);
	} else if (charId == 0x0BAE) {
		return ss4PlayerName;
	} else if (charId == 0x0B55) {
		return ss4CharacterNames0B55[charId-0x0B55];
	} else if (charId == 0x0A29 || charId == 0x0A2A) {
		return ss4CharacterNames0A29[charId-0x0A29];
	} else if (charId >= 0x09C5 && charId <= 0x09C9) {
		return ss4CharacterNames09C5[charId-0x09C5];
	} else if (charId == 0x05DC || charId == 0x05DD) {
		return ss4CharacterNamesInst[charId-0x05DC];
	} else if (charId >= 0x044C && charId <= 0x0469) {
		return ss4CharacterNamesMaleP3[charId-0x044C];
	} else if (charId >= 0x041A && charId <= 0x0437) {
		return ss4CharacterNamesMaleP2[charId-0x041A];
	} else if (charId >= 0x03E8 && charId <= 0x0408) {
		return ss4CharacterNamesMale[charId-0x03E8];
	} else if (charId >= 0x0352 && charId <= 0x036F) {
		return ss3CharacterNames5S[charId-0x0352];
	} else if (charId >= 0x0320 && charId <= 0x033D) {
		return ss4CharacterNamesReps[charId-0x0320];
	} else if (charId >= 0x02EE && charId <= 0x030B) {
		return ss4CharacterNamesP15[charId-0x02EE];
	} else if (charId >= 0x02BC && charId <= 0x02D9) {
		return ss3CharacterNamesP15[charId-0x02BC];
	} else if (charId >= 0x028A && charId <= 0x02A7) {
		return ss4CharacterNamesP13[charId-0x028A];
	} else if (charId >= 0x0258 && charId <= 0x0275) {
		return ss4CharacterNamesP12[charId-0x0258];
	} else if (charId >= 0x0226 && charId <= 0x0243) {
		return ss3CharacterNamesP12[charId-0x0226];
	} else if (charId >= 0x01F4 && charId <= 0x0211) {
		return ss4CharacterNamesP10[charId-0x01F4];
	} else if (charId >= 0x01C2 && charId <= 0x01DF) {
		return ss4CharacterNamesP9[charId-0x01C2];
	} else if (charId >= 0x0190 && charId <= 0x01AD) {
		return ss4CharacterNamesP8[charId-0x0190];
	} else if (charId >= 0x015E && charId <= 0x017B) {
		return ss4CharacterNamesP7[charId-0x015E];
	} else if (charId >= 0x012C && charId <= 0x0149) {
		return ss4CharacterNamesP6[charId-0x012C];
	} else if (charId >= 0x00FA && charId <= 0x0117) {
		return ss4CharacterNamesP5[charId-0x00FA];
	} else if (charId >= 0x00C8 && charId <= 0xE5) {
		return ss4CharacterNamesP4[charId-0x00C8];
	} else if (charId >= 0x0096 && charId <= 0xB3) {
		return ss4CharacterNamesP3[charId-0x0096];
	} else if (charId >= 0x0063 && charId <= 0x81) {
		return ss4CharacterNamesP2[charId-0x0063];
	} else if (charId <= 0x005E) {
		return ss4CharacterNames[charId];
	}
	return "???";
}

static const char* poseSetNames[] = {
	"Active",
	"Cute",
	"Cool",
};

extern bool ss3DLCharactersBackedUp;

static u16 currentCharId = 0;
static u8 currentHeight = 0;
static u8 currentPoseSet = 0;

CharacterChange::CharacterChange() {
	getList();
}

void CharacterChange::getList() {
	peopleMet = 0;
	if (highlightedGame == 3) {
		characterChangeMenuOps[0] = 2;
		characterChangeMenuOps[1] = 0;
		characterChangeMenuOps[2] = 5;
		characterChangeMenuOps[3] = 10;
		characterChangeMenuOptions = 3;
		totalCharacters = 0;
		readSS4Save();
		int i = 0;
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_AtoB)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_AtoB[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_CtoD)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_CtoD[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_EtoF)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_EtoF[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_GtoI)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_GtoI[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_JtoL)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_JtoL[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_MtoN)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_MtoN[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_OtoP)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_OtoP[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_QtoS)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_QtoS[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_TtoV)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_TtoV[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss4CharacterOrder_WtoZ)/sizeof(u16); i++) {
			if (existsSS4Character(ss4CharacterOrder_WtoZ[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < 30; i++) {
			if (existsSS4Character(0x0BB9+i)) {
				peopleMet++;
			}
		}
	} else if (highlightedGame == 2) {
		characterChangeMenuOps[0] = 2;
		characterChangeMenuOps[1] = 5;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0;
		readSS3Save();
		int i = 0;
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_AtoB)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_AtoB[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_CtoD)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_CtoD[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_EtoF)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_EtoF[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_GtoI)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_GtoI[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_JtoL)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_JtoL[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_MtoN)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_MtoN[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_OtoP)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_OtoP[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_QtoS)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_QtoS[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_TtoV)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_TtoV[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < (int)sizeof(ss3CharacterOrder_WtoZ)/sizeof(u16); i++) {
			if (existsSS3Character(ss3CharacterOrder_WtoZ[i])) {
				peopleMet++;
			}
		}
		for (i = 0; i < 50; i++) {
			if (existsSS3Character(0x0BB9+i)) {
				peopleMet++;
			}
		}
	} else if (highlightedGame == 1) {
		characterChangeMenuOps[0] = 2;
		characterChangeMenuOps[1] = 4;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0;
		readSS2Save();
	}
	characterPage[highlightedGame] = 0;

	getMaxChars();
}

void CharacterChange::getMaxChars() {
	if (subScreenMode == 6 ? importFromSave_highlightedGame == 3 : highlightedGame == 3) {
		switch (subScreenMode == 6 ? importFromSave_characterPage[3] : characterPage[3]) {
			case 0:
			default:
				totalCharacters = 2;
				break;
			case 1:
				totalCharacters = (int)sizeof(ss4CharacterOrder_AtoB)/sizeof(u16);
				break;
			case 2:
				totalCharacters = (int)sizeof(ss4CharacterOrder_CtoD)/sizeof(u16);
				break;
			case 3:
				totalCharacters = (int)sizeof(ss4CharacterOrder_EtoF)/sizeof(u16);
				break;
			case 4:
				totalCharacters = (int)sizeof(ss4CharacterOrder_GtoI)/sizeof(u16);
				break;
			case 5:
				totalCharacters = (int)sizeof(ss4CharacterOrder_JtoL)/sizeof(u16);
				break;
			case 6:
				totalCharacters = (int)sizeof(ss4CharacterOrder_MtoN)/sizeof(u16);
				break;
			case 7:
				totalCharacters = (int)sizeof(ss4CharacterOrder_OtoP)/sizeof(u16);
				break;
			case 8:
				totalCharacters = (int)sizeof(ss4CharacterOrder_QtoS)/sizeof(u16);
				break;
			case 9:
				totalCharacters = (int)sizeof(ss4CharacterOrder_TtoV)/sizeof(u16);
				break;
			case 10:
				totalCharacters = (int)sizeof(ss4CharacterOrder_WtoZ)/sizeof(u16);
				break;
			case 11:
				totalCharacters = 30;
				break;
		}
		if (totalCharacters > 0) totalCharacters--;
	} else if (subScreenMode == 6 ? importFromSave_highlightedGame == 2 : highlightedGame == 2) {
		switch (subScreenMode == 6 ? importFromSave_characterPage[2] : characterPage[2]) {
			case 0:
			default:
				totalCharacters = 0;
				break;
			case 1:
				totalCharacters = (int)sizeof(ss3CharacterOrder_AtoB)/sizeof(u16);
				break;
			case 2:
				totalCharacters = (int)sizeof(ss3CharacterOrder_CtoD)/sizeof(u16);
				break;
			case 3:
				totalCharacters = (int)sizeof(ss3CharacterOrder_EtoF)/sizeof(u16);
				break;
			case 4:
				totalCharacters = (int)sizeof(ss3CharacterOrder_GtoI)/sizeof(u16);
				break;
			case 5:
				totalCharacters = (int)sizeof(ss3CharacterOrder_JtoL)/sizeof(u16);
				break;
			case 6:
				totalCharacters = (int)sizeof(ss3CharacterOrder_MtoN)/sizeof(u16);
				break;
			case 7:
				totalCharacters = (int)sizeof(ss3CharacterOrder_OtoP)/sizeof(u16);
				break;
			case 8:
				totalCharacters = (int)sizeof(ss3CharacterOrder_QtoS)/sizeof(u16);
				break;
			case 9:
				totalCharacters = (int)sizeof(ss3CharacterOrder_TtoV)/sizeof(u16);
				break;
			case 10:
				totalCharacters = (int)sizeof(ss3CharacterOrder_WtoZ)/sizeof(u16);
				break;
			case 11:
				//totalCharacters = 67;
				totalCharacters = 50;
				break;
		}
		if (totalCharacters > 0) totalCharacters--;
	}

	if (import_highlightedGame == 4) {
		import_totalCharacters = numberOfExportedCharacters-1;
	} else if (import_highlightedGame == 3) {
		import_totalCharacters = 0xD;
	} else if (import_highlightedGame == 2) {
		import_totalCharacters = 0x10;
	} else if (import_highlightedGame == 1) {
		import_totalCharacters = 0x13;
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
	if (characterPage[highlightedGame] == 0) {
		currentCharId = (highlightedGame == 3 ? 0x0BAE : 0x07D1);
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
			switch (characterPage[2]) {
				case 0:
					currentCharId = 0;
					break;
				case 1:
					currentCharId = ss3CharacterOrder_AtoB[characterList_cursorPosition];
					break;
				case 2:
					currentCharId = ss3CharacterOrder_CtoD[characterList_cursorPosition];
					break;
				case 3:
					currentCharId = ss3CharacterOrder_EtoF[characterList_cursorPosition];
					break;
				case 4:
					currentCharId = ss3CharacterOrder_GtoI[characterList_cursorPosition];
					break;
				case 5:
					currentCharId = ss3CharacterOrder_JtoL[characterList_cursorPosition];
					break;
				case 6:
					currentCharId = ss3CharacterOrder_MtoN[characterList_cursorPosition];
					break;
				case 7:
					currentCharId = ss3CharacterOrder_OtoP[characterList_cursorPosition];
					break;
				case 8:
					currentCharId = ss3CharacterOrder_QtoS[characterList_cursorPosition];
					break;
				case 9:
					currentCharId = ss3CharacterOrder_TtoV[characterList_cursorPosition];
					break;
				case 10:
					currentCharId = ss3CharacterOrder_WtoZ[characterList_cursorPosition];
					break;
				case 11:
					currentCharId = 0x0BB9+characterList_cursorPosition;
					break;
			}
			return getSS3CharName(currentCharId);
		case 3:
			switch (characterPage[3]) {
				case 0:
					currentCharId = 0;
					break;
				case 1:
					currentCharId = ss4CharacterOrder_AtoB[characterList_cursorPosition];
					break;
				case 2:
					currentCharId = ss4CharacterOrder_CtoD[characterList_cursorPosition];
					break;
				case 3:
					currentCharId = ss4CharacterOrder_EtoF[characterList_cursorPosition];
					break;
				case 4:
					currentCharId = ss4CharacterOrder_GtoI[characterList_cursorPosition];
					break;
				case 5:
					currentCharId = ss4CharacterOrder_JtoL[characterList_cursorPosition];
					break;
				case 6:
					currentCharId = ss4CharacterOrder_MtoN[characterList_cursorPosition];
					break;
				case 7:
					currentCharId = ss4CharacterOrder_OtoP[characterList_cursorPosition];
					break;
				case 8:
					currentCharId = ss4CharacterOrder_QtoS[characterList_cursorPosition];
					break;
				case 9:
					currentCharId = ss4CharacterOrder_TtoV[characterList_cursorPosition];
					break;
				case 10:
					currentCharId = ss4CharacterOrder_WtoZ[characterList_cursorPosition];
					break;
				case 11:
					currentCharId = 0x0BB9+characterList_cursorPosition;
					break;
			}
			return getSS4CharName(currentCharId);
	}
	return "???";
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
	extern int ss1Region;
	switch (ss1Region) {
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
	switch (saveRegion[1]) {
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
	switch (saveRegion[2]) {
		default:
			return "Style Savvy: Fashion Forward";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique 2: Fashion Forward";
		case CFG_REGION_JPN:
			return "Girls Mode 3: Kirakira * Code";
		case CFG_REGION_KOR:
			return "Girls Style: Glitter * Code";
	}
}

const char* CharacterChange::ss4Title(void) const {
	switch (saveRegion[3]) {
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
	if (messageNo == 7) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Shading has been applied.");
	} else if (messageNo == 6) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Characters from the 1st, 2nd, and");
		Gui::DrawStringCentered(0, 78, 0.60, BLACK, "4th games, will leave from the 3rd.");
		Gui::DrawStringCentered(0, 104, 0.60, BLACK, "Characters part of downloaded");
		Gui::DrawStringCentered(0, 124, 0.60, BLACK, "Caprice Chalet rooms (if exists)");
		Gui::DrawStringCentered(0, 144, 0.60, BLACK, "will be restored. Is this OK?");
	} else if (messageNo == 5) {
		Gui::DrawStringCentered(0, 68, 0.60, BLACK, "New characters have arrived!");
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

	for (int i = 0; i < 50; i++) {
		const char* charName = import_everyCharacterNames[i];
		const char* profileName = import_everyCharacterNames[i];
		if (sysRegion == CFG_REGION_JPN || sysRegion == CFG_REGION_KOR) {
			charName = import_everyCharacterNamesJAP[i];
			profileName = import_everyCharacterNamesJAP[i];
		}
		if (sysRegion == CFG_REGION_EUR || sysRegion == CFG_REGION_AUS) {
			profileName = import_everyCharacterProfileNamesEUR[i];
		}
		sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", charName);
		if (access(chrFilePath, F_OK) != 0) {
			sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), charName);
		}
		readSS3CharacterFile(0x0BB9+i, chrFilePath);
		if (removeBags) {
			removeSS3CharacterBag(0x0BB9+i);
		}
		sprintf(chrFilePath, "romfs:/character/Fashion Forward/Profiles/%s.cprf", profileName);
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
	remove("sdmc:/3ds/SavvyManager/SS3/dlCharacters.bak");
	ss3DLCharactersBackedUp = false;

	sprintf(chararacterImported, "Characters have been removed.");
	for (u16 id = 0x0BB9; id <= 0x0BFD; id++) {
		if (existsSS3Character(id)) {
			sprintf(chararacterImported, "Character(s) have been restored.");
			break;
		}
	}
}


void CharacterChange::Draw(void) const {
	GFX::unloadGameSelSheets();

	Gui::ScreenDraw(Top);

	//GFX::showBgSprite(zoomIn);
	if (highlightedGame == 3 && !previewCharacter) {
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(247, 206, 164, 255));
		bool light = true;
		int x = 0;
		int width = 29;
		for (int i = 0; i < 14; i++) {
			if (light) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(247, 231, 206, 255));
			}
			light = !light;
			x += width;
			width = light ? 29 : 28;
		}
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}
	if (previewCharacter) {
		if (previewCharacterFound) {
			GFX::showCharSprite(zoomIn, charFadeAlpha);
		} else {
			Gui::DrawStringCentered(0, 104, 0.65, BLACK, (import_highlightedGame==4 ? "Preview not found." : "Preview unavailable."));
		}
	} else if (subScreenMode == 0) {
		if (!showMessage) {
			if (assistantChange) {
				Gui::DrawStringCentered(0, 64, 0.50, BLACK, "Choose who you want to be your");
				Gui::DrawStringCentered(0, 78, 0.50, BLACK, "shop assistant.");
				Gui::DrawStringCentered(0, 104, 0.50, BLACK, "Anyone other than Xin, Sylvia, Hannah,");
				Gui::DrawStringCentered(0, 118, 0.50, BLACK, "Ken, Archie, or Rahil, will not have");
				Gui::DrawStringCentered(0, 132, 0.50, BLACK, "their card shown in your shop.");
			} else {
				Gui::DrawStringCentered(0, 104, 0.50, BLACK, "Select the character you want to change.");
			}
		}
		if (peopleMet > 0 && peopleMetCount) {
			if (highlightedGame == 3) {
				Gui::Draw_Rect(0, 208, 400, 32, WHITE);
			}
			char peopleMetText[24];
			sprintf(peopleMetText, "People Met: %i", peopleMet);
			Gui::DrawStringCentered(0, 216, 0.60, BLACK, peopleMetText);
		}
	}

	preview();

	if (showMessage && messageNo == 4) {
		Gui::DrawString(8, 210, 0.50, BLACK, removeBags ? " Keep bags" : " Remove bags");
		// Selected season
		Gui::DrawString(160, 208, 0.65, BLACK, "L");
		Gui::DrawStringCentered(0, 210, 0.50, BLACK, seasonName());
		Gui::DrawString(232, 208, 0.65, BLACK, "R");
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) {
		Gui::Draw_Rect(0, 239, 400, 1, C2D_Color32(0, 0, 0, 255));
		return;
	}
	Gui::ScreenDraw(Bottom);
	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(247, 214, 181, 255));
		bool dark = true;
		int x = 0;
		int width = 17;
		for (int i = 0; i < 14; i++) {
			if (dark) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(247, 206, 164, 255));
			}
			dark = !dark;
			x += width;
			width = dark ? 29 : 28;
		}
		Gui::Draw_Rect(0, 0, 320, 33, C2D_Color32(247, 235, 206, 255));
		Gui::Draw_Rect(0, 33, 320, 5, C2D_Color32(214, 158, 58, 255));
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}

	cursorX = 248;
	if (subScreenMode == 7) {
		cursorY = 64+(48*shadeChange_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the shading to use for every character.");

		int i2 = (highlightedGame == 3 ? 8 : 0);
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Cel-shade (Original)");
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "No cel-shade");
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Fashion Dreamer");
	} else if (subScreenMode == 5) {
		cursorY = 64+(48*importWhereList_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, "Import from where?");

		int i2 = (highlightedGame == 3 ? 8 : 0);
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Savvy Manager");
		if (highlightedGame > 1) {
			i2 += 48;
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2-8, 0.50, BLACK, "Save data of");
			bool saveFound = false;
			char gameTitle[64];
			switch (importFromSave_highlightedGame) {
				case 0:
					sprintf(gameTitle, "< %s >", ss1Title());
					break;
				case 1:
					saveFound = ss2SaveFound;
					sprintf(gameTitle, "< %s >", ss2Title());
					break;
				case 2:
					saveFound = ss3SaveFound;
					sprintf(gameTitle, "< %s >", ss3Title());
					break;
				case 3:
					saveFound = ss4SaveFound;
					sprintf(gameTitle, "< %s >", ss4Title());
					break;
			}
			Gui::DrawString(32, i2+8, 0.50, saveFound ? BLACK : HALF_BLACK, gameTitle);
		}
	} else if (subScreenMode == 4) {
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
		char chrCounter[24];
		sprintf(chrCounter, "%d/%d", importCharacterList_cursorPosition+1, import_totalCharacters+1);
		Gui::DrawString(64, 184, 0.55, BLACK, chrCounter);

		int i2 = (highlightedGame == 3 ? 56 : 48);
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
	} else if (subScreenMode == 2) {
		cursorY = 64+(48*characterAttributeList_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, characterName(true));
		bool isMale = false;
		if (highlightedGame == 3) {
			isMale = getSS4CharacterGenderNoExceptions(currentCharId);
			currentHeight = readSS4CharacterHeight(currentCharId);
			currentPoseSet = readSS4CharacterPoseSet(currentCharId);
		} else if (highlightedGame == 2) {
			isMale = getSS3CharacterGenderNoExceptions(currentCharId);
			currentHeight = readSS3CharacterHeight(currentCharId);
			currentPoseSet = readSS3CharacterPoseSet(currentCharId);
		} else if (highlightedGame == 1) {
			isMale = getSS2CharacterGenderNoExceptions();
			currentHeight = readSS2CharacterHeight();
			currentPoseSet = readSS2CharacterPoseSet();
		}

		char heightString[24];
		if (currentHeight == 0) {
			sprintf(heightString, "Height: < Invisible >");
		} else {
			sprintf(heightString, "Height: < %i >", currentHeight);
		}

		char poseSetString[24];
		sprintf(poseSetString, "Pose Set: < %s >", poseSetNames[currentPoseSet-1]);

		int i2 = (highlightedGame == 3 ? 8 : 0);
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		GFX::DrawSprite((isMale ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
		Gui::DrawString(64, i2, 0.65, BLACK, (highlightedGame == 3) ? (isMale ? "Gender: < Male >" : "Gender: < Female >") : (isMale ? "Gender: Male" : "Gender: Female"));
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(64, i2, 0.65, BLACK, heightString);
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(64, i2, 0.65, BLACK, poseSetString);
	} else if (subScreenMode == 1) {
		cursorY = 64+(48*characterChangeMenu_cursorPositionOnScreen);

		Gui::DrawString(8, 8, 0.50, BLACK, characterName(true));

		int i2 = (highlightedGame == 3 ? 8 : 0);
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
		int currentGame = highlightedGame;
		int currentCharShownFirst = characterShownFirst;
		int currentCharPage = characterPage[currentGame];
		if (subScreenMode == 6) {
			cursorY = 64+(48*importFromSave_cursorPositionOnScreen);
			currentGame = importFromSave_highlightedGame;
			currentCharShownFirst = importFromSave_characterShownFirst;
			currentCharPage = importFromSave_characterPage[currentGame];
		}

		const char* letterText[] =    { "A", "C", "E", "G", "J", "M", "O", "Q", "T", "W"};
		const char* letterTextBot[] = {"-B","-D","-F","-I","-L","-N","-P","-S","-V","-Z"};
		Gui::DrawString(8, 10, 0.50, currentCharPage==0 ? RED : (assistantChange ? HALF_BLACK : BLACK), "Main");
		for (int i = 0; i < 10; i++) {
			Gui::DrawString(40+(i*24), 4, 0.50, (currentCharPage > 0 && currentCharPage-1 == i) ? RED : BLACK, letterText[i]);
			Gui::DrawString(48+(i*24), 16, 0.50, (currentCharPage > 0 && currentCharPage-1 == i) ? RED : BLACK, letterTextBot[i]);
		}
		Gui::DrawString(292, 10, 0.50, currentCharPage==11 ? RED : (assistantChange ? HALF_BLACK : BLACK), "Ext.");

		if (subScreenMode == 0) {
			if (highlightedGame == 3) {
				Gui::DrawString(116, 210, 0.50, BLACK, "START: Change shading");
			} else if (highlightedGame == 2) {
				Gui::DrawString(116, 210, 0.50, BLACK, ss3DLCharactersBackedUp ? "START: Remove contacts" : "START: Expand contacts");
			}
		}

	  if (!displayNothing) {
		char chrCounter[24];
		sprintf(chrCounter, "%d/%d", (subScreenMode == 6 ? importFromSave_cursorPosition : characterList_cursorPosition)+1, totalCharacters+1);
		Gui::DrawString(64, 184, 0.55, BLACK, chrCounter);

		int i2 = (highlightedGame == 3 ? 56 : 48);
		for (int i = currentCharShownFirst; i < currentCharShownFirst+3; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (currentGame == 3) {
				if (currentCharPage == 0) {
					if (i == 1) {
						u16 charId = getSS4AssistantCharacterId();
						GFX::DrawSprite((getSS4CharacterGender(charId) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
						Gui::DrawString(64, i2-8, 0.50, BLACK, "Shop Assistant");
						Gui::DrawString(64, i2+8, 0.65, BLACK, getSS4CharName(charId));
						break;
					} else {
						GFX::DrawSprite((getSS4CharacterGender(0xBAE) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
						Gui::DrawString(64, i2, 0.65, BLACK, ss4PlayerName);
					}
				} else {
					u16 charId = 0;
					switch (currentCharPage) {
						case 1:
							charId = ss4CharacterOrder_AtoB[i];
							break;
						case 2:
							charId = ss4CharacterOrder_CtoD[i];
							break;
						case 3:
							charId = ss4CharacterOrder_EtoF[i];
							break;
						case 4:
							charId = ss4CharacterOrder_GtoI[i];
							break;
						case 5:
							charId = ss4CharacterOrder_JtoL[i];
							break;
						case 6:
							charId = ss4CharacterOrder_MtoN[i];
							break;
						case 7:
							charId = ss4CharacterOrder_OtoP[i];
							break;
						case 8:
							charId = ss4CharacterOrder_QtoS[i];
							break;
						case 9:
							charId = ss4CharacterOrder_TtoV[i];
							break;
						case 10:
							charId = ss4CharacterOrder_WtoZ[i];
							break;
						case 11:
							charId = 0x0BB9+i;
							break;
					}
					int charFlag = 0;
					const char* charInfo = "";
					if (charId >= 0x0BB9) {
					} else if (charId == 0x0B55) {
						charFlag = ss4CharacterFlags30_3[charId-0x0B55];
					} else if (charId >= 0x0A29) {
						charFlag = ss4CharacterFlags30_3[charId-0x0A29];
					} else if (charId >= 0x09C5) {
						charFlag = ss4CharacterFlags30_3[charId-0x09C5];
					} else if (charId >= 0x05DC) {
						charFlag = ss4CharacterFlags30_0[charId-0x05DC];
					} else if (charId >= 0x044C) {
						charFlag = ss4CharacterFlags30_0[charId-0x044C];
					} else if (charId >= 0x041A) {
						charFlag = ss4CharacterFlagsMaleP2[charId-0x041A];
					} else if (charId >= 0x03E8) {
						charFlag = ss4CharacterFlagsMale[charId-0x03E8];
					} else if (charId >= 0x0352) {
						charFlag = ss4CharacterFlags30_3[charId-0x0352];
					} else if (charId >= 0x0320) {
						charFlag = ss4CharacterFlagsReps[charId-0x0320];
					} else if (charId >= 0x02EE) {
						charFlag = ss4CharacterFlags30_0[charId-0x02EE];
					} else if (charId >= 0x02BC) {
						charFlag = ss4CharacterFlags30_3[charId-0x02BC];
					} else if (charId >= 0x028A) {
						charFlag = ss4CharacterFlags30_0[charId-0x028A];
					} else if (charId >= 0x0258) {
						charFlag = ss4CharacterFlags30_0[charId-0x0258];
					} else if (charId >= 0x0226) {
						charFlag = ss4CharacterFlags30_0[charId-0x0226];
					} else if (charId >= 0x01F4) {
						charFlag = ss4CharacterFlags30_0[charId-0x01F4];
					} else if (charId >= 0x01C2) {
						charFlag = ss4CharacterFlags30_0[charId-0x01C2];
					} else if (charId >= 0x0190) {
						charFlag = ss4CharacterFlags30_0[charId-0x0190];
					} else if (charId >= 0x015E) {
						charFlag = ss4CharacterFlags30_0[charId-0x015E];
					} else if (charId >= 0x012C) {
						charFlag = ss4CharacterFlags30_0[charId-0x012C];
					} else if (charId >= 0x00FA) {
						charFlag = ss4CharacterFlags30_0[charId-0x00FA];
					} else if (charId >= 0x00C8) {
						charFlag = ss4CharacterFlags30_0[charId-0x00C8];
					} else if (charId >= 0x0096) {
						charFlag = ss4CharacterFlags30_0[charId-0x0096];
					} else if (charId >= 0x0063) {
						charFlag = ss4CharacterFlagsP2[charId-0x0063];
					} else {
						charFlag = ss4CharacterFlags[charId];
					}
					switch (charFlag) {
						default:
							break;
						case 1:
							charInfo = "Not in Contacts";
							break;
						case 2:
							charInfo = "Unseen";
							break;
						case 3:
							charInfo = "Fashion Forward Leftover";
							break;
					}
					GFX::DrawSprite((getSS4CharacterGender(charId) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					if (charFlag > 0) {
						Gui::DrawString(64, i2-12, 0.50, RED, charInfo);
					}
					Gui::DrawString(64, i2, 0.65, existsSS4Character(charId) ? BLACK : HALF_BLACK, getSS4CharName(charId));
				}
			} else if (currentGame == 2) {
				if (currentCharPage == 0) {
					GFX::DrawSprite((getSS3CharacterGender(0x7D1) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, BLACK, ss3PlayerName);
					break;
				} else {
					u16 charId = 0;
					switch (currentCharPage) {
						case 1:
							charId = ss3CharacterOrder_AtoB[i];
							break;
						case 2:
							charId = ss3CharacterOrder_CtoD[i];
							break;
						case 3:
							charId = ss3CharacterOrder_EtoF[i];
							break;
						case 4:
							charId = ss3CharacterOrder_GtoI[i];
							break;
						case 5:
							charId = ss3CharacterOrder_JtoL[i];
							break;
						case 6:
							charId = ss3CharacterOrder_MtoN[i];
							break;
						case 7:
							charId = ss3CharacterOrder_OtoP[i];
							break;
						case 8:
							charId = ss3CharacterOrder_QtoS[i];
							break;
						case 9:
							charId = ss3CharacterOrder_TtoV[i];
							break;
						case 10:
							charId = ss3CharacterOrder_WtoZ[i];
							break;
						case 11:
							charId = 0x0BB9+i;
							break;
					}
					GFX::DrawSprite((getSS3CharacterGender(charId) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Gui::DrawString(64, i2, 0.65, existsSS3Character(charId) ? BLACK : HALF_BLACK, getSS3CharName(charId));
				}
			} else if (currentGame == 1) {
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



void CharacterChange::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
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
				peopleMetCount = true;
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
				sndBack();
				showMessage = false;
				peopleMetCount = true;
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
					sprintf(chararacterImported, "Contacts have been expanded.");
				} else {
					if (subScreenMode == 1) {
						previewCharacter = false;
					}
					showMessage = false;
				}
			}
		}
	} else if (subScreenMode == 7) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				shadeChange_cursorPosition--;
				if (shadeChange_cursorPosition < 0) {
					shadeChange_cursorPosition = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				shadeChange_cursorPosition++;
				if (shadeChange_cursorPosition > 2) {
					shadeChange_cursorPosition = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			loadCommonLut(shadeChange_cursorPosition);
			messageNo = 7;
			showMessage = true;
			subScreenMode = 0;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 0;
		}

	} else if (subScreenMode == 6) {
		if (showCursor) {
			if ((hDownRepeat & KEY_DUP) && (importFromSave_highlightedGame > 1) && (importFromSave_characterPage[importFromSave_highlightedGame] > 0)) {
				sndHighlight();
				importFromSave_cursorPosition--;
				importFromSave_cursorPositionOnScreen--;
				if (importFromSave_cursorPosition < 0) {
					importFromSave_cursorPosition = 0;
					importFromSave_characterShownFirst = 0;
				} else if (importFromSave_cursorPosition < importFromSave_characterShownFirst) {
					importFromSave_characterShownFirst--;
				}
				if (importFromSave_cursorPositionOnScreen < 0) {
					importFromSave_cursorPositionOnScreen = 0;
				}
				getMaxChars();
			}

			if ((hDownRepeat & KEY_DDOWN) && (importFromSave_highlightedGame > 1) && (importFromSave_characterPage[importFromSave_highlightedGame] > 0)) {
				sndHighlight();
				importFromSave_cursorPosition++;
				importFromSave_cursorPositionOnScreen++;
				if (importFromSave_cursorPosition > totalCharacters) {
					importFromSave_cursorPosition = totalCharacters;
					importFromSave_characterShownFirst = totalCharacters-2;
				} else if (importFromSave_cursorPosition > importFromSave_characterShownFirst+2) {
					importFromSave_characterShownFirst++;
				}
				if (importFromSave_cursorPositionOnScreen > 2) {
					importFromSave_cursorPositionOnScreen = 2;
				}
			}

			if ((hDown & KEY_DLEFT) && (highlightedGame > 1)) {
				sndHighlight();
				importFromSave_characterPage[importFromSave_highlightedGame]--;
				if (importFromSave_characterPage[importFromSave_highlightedGame] < 0) importFromSave_characterPage[importFromSave_highlightedGame] = 0;
			}

			if ((hDown & KEY_DRIGHT) && (highlightedGame > 1)) {
				sndHighlight();
				importFromSave_characterPage[importFromSave_highlightedGame]++;
				if (importFromSave_characterPage[importFromSave_highlightedGame] > 11) importFromSave_characterPage[importFromSave_highlightedGame] = 11;
			}

			if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
				importFromSave_cursorPosition = 0;
				importFromSave_cursorPositionOnScreen = 0;
				importFromSave_characterShownFirst = 0;
				getMaxChars();
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			u16 charId = 0;
			if (highlightedGame == 3) {
				if (importFromSave_highlightedGame == 3) {
					switch (importFromSave_characterPage[3]) {
						case 0:
						default:
							charId = 0x0BAE;
							break;
						case 1:
							charId = ss4CharacterOrder_AtoB[importFromSave_cursorPosition];
							break;
						case 2:
							charId = ss4CharacterOrder_CtoD[importFromSave_cursorPosition];
							break;
						case 3:
							charId = ss4CharacterOrder_EtoF[importFromSave_cursorPosition];
							break;
						case 4:
							charId = ss4CharacterOrder_GtoI[importFromSave_cursorPosition];
							break;
						case 5:
							charId = ss4CharacterOrder_JtoL[importFromSave_cursorPosition];
							break;
						case 6:
							charId = ss4CharacterOrder_MtoN[importFromSave_cursorPosition];
							break;
						case 7:
							charId = ss4CharacterOrder_OtoP[importFromSave_cursorPosition];
							break;
						case 8:
							charId = ss4CharacterOrder_QtoS[importFromSave_cursorPosition];
							break;
						case 9:
							charId = ss4CharacterOrder_TtoV[importFromSave_cursorPosition];
							break;
						case 10:
							charId = ss4CharacterOrder_WtoZ[importFromSave_cursorPosition];
							break;
						case 11:
							charId = 0x0BB9+importFromSave_cursorPosition;
							break;
					}
					readSS4Character(charId);
					sprintf(chararacterImported, "%s imported.", getSS4CharName(charId));
				} else {
					switch (importFromSave_characterPage[2]) {
						case 0:
						default:
							charId = 0x07D1;
							break;
						case 1:
							charId = ss3CharacterOrder_AtoB[importFromSave_cursorPosition];
							break;
						case 2:
							charId = ss3CharacterOrder_CtoD[importFromSave_cursorPosition];
							break;
						case 3:
							charId = ss3CharacterOrder_EtoF[importFromSave_cursorPosition];
							break;
						case 4:
							charId = ss3CharacterOrder_GtoI[importFromSave_cursorPosition];
							break;
						case 5:
							charId = ss3CharacterOrder_JtoL[importFromSave_cursorPosition];
							break;
						case 6:
							charId = ss3CharacterOrder_MtoN[importFromSave_cursorPosition];
							break;
						case 7:
							charId = ss3CharacterOrder_OtoP[importFromSave_cursorPosition];
							break;
						case 8:
							charId = ss3CharacterOrder_QtoS[importFromSave_cursorPosition];
							break;
						case 9:
							charId = ss3CharacterOrder_TtoV[importFromSave_cursorPosition];
							break;
						case 10:
							charId = ss3CharacterOrder_WtoZ[importFromSave_cursorPosition];
							break;
						case 11:
							charId = 0x0BB9+importFromSave_cursorPosition;
							break;
					}
					readSS3CharacterToSS4(charId);
					sprintf(chararacterImported, "%s imported.", getSS3CharName(charId));
				}
				switch (characterPage[3]) {
					case 0:
					default:
						charId = 0x0BAE;
						break;
					case 1:
						charId = ss4CharacterOrder_AtoB[characterList_cursorPosition];
						break;
					case 2:
						charId = ss4CharacterOrder_CtoD[characterList_cursorPosition];
						break;
					case 3:
						charId = ss4CharacterOrder_EtoF[characterList_cursorPosition];
						break;
					case 4:
						charId = ss4CharacterOrder_GtoI[characterList_cursorPosition];
						break;
					case 5:
						charId = ss4CharacterOrder_JtoL[characterList_cursorPosition];
						break;
					case 6:
						charId = ss4CharacterOrder_MtoN[characterList_cursorPosition];
						break;
					case 7:
						charId = ss4CharacterOrder_OtoP[characterList_cursorPosition];
						break;
					case 8:
						charId = ss4CharacterOrder_QtoS[characterList_cursorPosition];
						break;
					case 9:
						charId = ss4CharacterOrder_TtoV[characterList_cursorPosition];
						break;
					case 10:
						charId = ss4CharacterOrder_WtoZ[characterList_cursorPosition];
						break;
					case 11:
						charId = 0x0BB9+characterList_cursorPosition;
						break;
				}
				writeSS4Character(charId);
				writeSS4CharacterToSave(charId);
				messageNo = 1;
				showMessage = true;
			} else if (highlightedGame == 2) {
				if (importFromSave_highlightedGame == 3) {
					switch (importFromSave_characterPage[3]) {
						case 0:
						default:
							charId = 0x0BAE;
							break;
						case 1:
							charId = ss4CharacterOrder_AtoB[importFromSave_cursorPosition];
							break;
						case 2:
							charId = ss4CharacterOrder_CtoD[importFromSave_cursorPosition];
							break;
						case 3:
							charId = ss4CharacterOrder_EtoF[importFromSave_cursorPosition];
							break;
						case 4:
							charId = ss4CharacterOrder_GtoI[importFromSave_cursorPosition];
							break;
						case 5:
							charId = ss4CharacterOrder_JtoL[importFromSave_cursorPosition];
							break;
						case 6:
							charId = ss4CharacterOrder_MtoN[importFromSave_cursorPosition];
							break;
						case 7:
							charId = ss4CharacterOrder_OtoP[importFromSave_cursorPosition];
							break;
						case 8:
							charId = ss4CharacterOrder_QtoS[importFromSave_cursorPosition];
							break;
						case 9:
							charId = ss4CharacterOrder_TtoV[importFromSave_cursorPosition];
							break;
						case 10:
							charId = ss4CharacterOrder_WtoZ[importFromSave_cursorPosition];
							break;
						case 11:
							charId = 0x0BB9+importFromSave_cursorPosition;
							break;
					}
					readSS4CharacterToSS3(charId);
					sprintf(chararacterImported, "%s imported.", getSS4CharName(charId));
				} else {
					switch (importFromSave_characterPage[2]) {
						case 0:
						default:
							charId = 0x07D1;
							break;
						case 1:
							charId = ss3CharacterOrder_AtoB[importFromSave_cursorPosition];
							break;
						case 2:
							charId = ss3CharacterOrder_CtoD[importFromSave_cursorPosition];
							break;
						case 3:
							charId = ss3CharacterOrder_EtoF[importFromSave_cursorPosition];
							break;
						case 4:
							charId = ss3CharacterOrder_GtoI[importFromSave_cursorPosition];
							break;
						case 5:
							charId = ss3CharacterOrder_JtoL[importFromSave_cursorPosition];
							break;
						case 6:
							charId = ss3CharacterOrder_MtoN[importFromSave_cursorPosition];
							break;
						case 7:
							charId = ss3CharacterOrder_OtoP[importFromSave_cursorPosition];
							break;
						case 8:
							charId = ss3CharacterOrder_QtoS[importFromSave_cursorPosition];
							break;
						case 9:
							charId = ss3CharacterOrder_TtoV[importFromSave_cursorPosition];
							break;
						case 10:
							charId = ss3CharacterOrder_WtoZ[importFromSave_cursorPosition];
							break;
						case 11:
							charId = 0x0BB9+importFromSave_cursorPosition;
							break;
					}
					readSS3Character(charId);
					sprintf(chararacterImported, "%s imported.", getSS3CharName(charId));
				}
				switch (characterPage[2]) {
					case 0:
					default:
						charId = 0x07D1;
						break;
					case 1:
						charId = ss3CharacterOrder_AtoB[characterList_cursorPosition];
						break;
					case 2:
						charId = ss3CharacterOrder_CtoD[characterList_cursorPosition];
						break;
					case 3:
						charId = ss3CharacterOrder_EtoF[characterList_cursorPosition];
						break;
					case 4:
						charId = ss3CharacterOrder_GtoI[characterList_cursorPosition];
						break;
					case 5:
						charId = ss3CharacterOrder_JtoL[characterList_cursorPosition];
						break;
					case 6:
						charId = ss3CharacterOrder_MtoN[characterList_cursorPosition];
						break;
					case 7:
						charId = ss3CharacterOrder_OtoP[characterList_cursorPosition];
						break;
					case 8:
						charId = ss3CharacterOrder_QtoS[characterList_cursorPosition];
						break;
					case 9:
						charId = ss3CharacterOrder_TtoV[characterList_cursorPosition];
						break;
					case 10:
						charId = ss3CharacterOrder_WtoZ[characterList_cursorPosition];
						break;
					case 11:
						charId = 0x0BB9+characterList_cursorPosition;
						break;
				}
				writeSS3Character(charId);
				writeSS3CharacterToSave(charId);
				messageNo = 1;
				showMessage = true;
			}
			subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			importFromSave_cursorPosition = 0;
			importFromSave_cursorPositionOnScreen = 0;
			importFromSave_characterShownFirst = 0;
			importFromSave_cursorPosition = 0;
			importFromSave_cursorPositionOnScreen = 0;
			subScreenMode = 5;
		}
	} else if (subScreenMode == 5) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				importWhereList_cursorPosition--;
				if (importWhereList_cursorPosition < 0) {
					importWhereList_cursorPosition = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				importWhereList_cursorPosition++;
				if (importWhereList_cursorPosition > 1) {
					importWhereList_cursorPosition = 1;
				}
			}
		}

		if ((hDown & KEY_DLEFT) && highlightedGame >= 2) {
			sndHighlight();
			importFromSave_highlightedGame--;
			if (importFromSave_highlightedGame < 2) importFromSave_highlightedGame = 2;
		}

		if ((hDown & KEY_DRIGHT) && highlightedGame >= 2) {
			sndHighlight();
			importFromSave_highlightedGame++;
			if (importFromSave_highlightedGame > 3) importFromSave_highlightedGame = 3;
		}

		if (hDown & KEY_A) {
			if (importWhereList_cursorPosition == 0) {
				sndSelect();
				displayNothing = true;
				subScreenMode = 4;
				if ((import_highlightedGame == 4) && !exportedCharListGotten[highlightedGame]) {
					gspWaitForVBlank();
					getExportedCharacterContents();
					exportedCharListGotten[highlightedGame] = true;
				}
				displayNothing = false;
				loadChrImage(false);
			} else if (importWhereList_cursorPosition == 1) {
				bool saveFound = false;
				switch (importFromSave_highlightedGame) {
					case 0:
					default:
						break;
					case 1:
						saveFound = ss2SaveFound;
						if (saveFound) {
							readSS2Save();
						}
						break;
					case 2:
						saveFound = ss3SaveFound;
						if (saveFound) {
							readSS3Save();
						}
						break;
					case 3:
						saveFound = ss4SaveFound;
						if (saveFound) {
							readSS4Save();
						}
						break;
				}
				if (saveFound) {
					sndSelect();
					subScreenMode = 6;
				} else {
					sndBack();
				}
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 1;
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
			if (hDownRepeat & KEY_DUP) {
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

			if (hDownRepeat & KEY_DDOWN) {
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
						readSS4CharacterFile(currentCharId, chrFilePath);
						writeSS4CharacterToSave(currentCharId);
						break;
					case 2:
						sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", "Robz");
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), "Robz");
						}
						readSS3CharacterFile(currentCharId, chrFilePath);
						writeSS3CharacterToSave(currentCharId);
						break;
					case 1:
						sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", "Robz");
						if (access(chrFilePath, F_OK) != 0) {
						sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), "Robz");
						}
						readSS2CharacterFile(chrFilePath);
						writeSS2CharacterToSave();
						break;
				}
				loadChrImage(true);
				sprintf(chararacterImported, "%s imported.", "Robz");
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
							readSS4CharacterFile(currentCharId, chrFilePath);
							writeSS4CharacterToSave(currentCharId);
							exportFound = true;
						}
						break;
					case 2:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS3CharacterFile(currentCharId, chrFilePath);
							writeSS3CharacterToSave(currentCharId);
							exportFound = true;
						}
						break;
					case 1:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", getExportedCharacterName(importCharacterList_cursorPosition));
						if (access(chrFilePath, F_OK) == 0) {
							sndSelect();
							readSS2CharacterFile(chrFilePath);
							writeSS2CharacterToSave();
							exportFound = true;
						}
						break;
				}
				if (exportFound) {
					sprintf(chararacterImported, "%s imported.", getExportedCharacterName(importCharacterList_cursorPosition));
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
						readSS4CharacterFile(currentCharId, chrFilePath);
						writeSS4CharacterToSave(currentCharId);
						break;
					case 2:
						sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS3CharacterFile(currentCharId, chrFilePath);
						writeSS3CharacterToSave(currentCharId);
						break;
					case 1:
						sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS2CharacterFile(chrFilePath);
						writeSS2CharacterToSave();
						break;
				}
				sprintf(chararacterImported, "%s imported.", import_characterNameDisplay());
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

	} else if (subScreenMode == 2) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				characterAttributeList_cursorPosition--;
				if (characterAttributeList_cursorPosition < 0) {
					characterAttributeList_cursorPosition = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				characterAttributeList_cursorPosition++;
				if (characterAttributeList_cursorPosition > 2) {
					characterAttributeList_cursorPosition = 2;
				}
			}

			if (hDown & KEY_DLEFT) {
				switch (characterAttributeList_cursorPosition) {
					case 0:
						if (highlightedGame == 3) {
							sndHighlight();
							changeSS4CharacterGender(currentCharId);
							changesMade = true;
						}
						break;
					case 1:
						sndHighlight();
						currentHeight--;
						if (highlightedGame == 3) {
							if (currentHeight < 1) currentHeight = 3;
						} else {
							if (currentHeight == 0xFF) currentHeight = 3;
						}
						if (highlightedGame == 3) {
							writeSS4CharacterHeight(currentCharId, currentHeight);
						} else if (highlightedGame == 2) {
							writeSS3CharacterHeight(currentCharId, currentHeight);
						} else if (highlightedGame == 1) {
							writeSS2CharacterHeight(currentHeight);
						}
						changesMade = true;
						break;
					case 2:
						sndHighlight();
						currentPoseSet--;
						if (currentPoseSet < 1) currentPoseSet = 3;
						if (highlightedGame == 3) {
							writeSS4CharacterPoseSet(currentCharId, currentPoseSet);
						} else if (highlightedGame == 2) {
							writeSS3CharacterPoseSet(currentCharId, currentPoseSet);
						} else if (highlightedGame == 1) {
							writeSS2CharacterPoseSet(currentPoseSet);
						}
						changesMade = true;
						break;
				}
			}

			if ((hDown & KEY_DRIGHT) || (hDown & KEY_A)) {
				switch (characterAttributeList_cursorPosition) {
					case 0:
						if (highlightedGame == 3) {
							sndHighlight();
							changeSS4CharacterGender(currentCharId);
							changesMade = true;
						}
						break;
					case 1:
						sndHighlight();
						currentHeight++;
						if (currentHeight > 3) currentHeight = (highlightedGame == 3) ? 1 : 0;
						if (highlightedGame == 3) {
							writeSS4CharacterHeight(currentCharId, currentHeight);
						} else if (highlightedGame == 2) {
							writeSS3CharacterHeight(currentCharId, currentHeight);
						} else if (highlightedGame == 1) {
							writeSS2CharacterHeight(currentHeight);
						}
						changesMade = true;
						break;
					case 2:
						sndHighlight();
						currentPoseSet++;
						if (currentPoseSet > 3) currentPoseSet = 1;
						if (highlightedGame == 3) {
							writeSS4CharacterPoseSet(currentCharId, currentPoseSet);
						} else if (highlightedGame == 2) {
							writeSS3CharacterPoseSet(currentCharId, currentPoseSet);
						} else if (highlightedGame == 1) {
							writeSS2CharacterPoseSet(currentPoseSet);
						}
						changesMade = true;
						break;
				}
			}

		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			if (changesMade) {
				if (highlightedGame == 3) {
					writeSS4CharacterToSave(currentCharId);
				} else if (highlightedGame == 2) {
					writeSS3CharacterToSave(currentCharId);
				} else if (highlightedGame == 1) {
					writeSS2Character();
					writeSS2CharacterToSave();
				}
				changesMade = false;
			}
			subScreenMode = 1;
		}

	} else if (subScreenMode == 1) {
		if (showCursor) {
			if (hDownRepeat & KEY_DUP) {
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

			if (hDownRepeat & KEY_DDOWN) {
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
						writeSS4CharacterFile(currentCharId, chrFilePath);
						break;
					case 2:
						sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", characterName(false));
						writeSS3CharacterFile(currentCharId, chrFilePath);
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
				if ((subScreenMode == 2) && (highlightedGame == 1)) {
					readSS2Character();
				} else if ((subScreenMode == 4) && (import_highlightedGame == 4) && !exportedCharListGotten[highlightedGame]) {
					gspWaitForVBlank();
					getExportedCharacterContents();
					exportedCharListGotten[highlightedGame] = true;
				}
				displayNothing = false;
				if (subScreenMode == 4) {
					loadChrImage(false);
				}
				if (subScreenMode == 5 && highlightedGame == 2) {
					importFromSave_highlightedGame = 2;
				}
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 0;
		}

	} else {
		if (showCursor) {
			if ((hDownRepeat & KEY_DUP) && (highlightedGame > 1) && (highlightedGame == 3 || characterPage[highlightedGame] > 0)) {
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
				getMaxChars();
			}

			if ((hDownRepeat & KEY_DDOWN) && (highlightedGame > 1) && (highlightedGame == 3 || characterPage[highlightedGame] > 0)) {
				sndHighlight();
				characterList_cursorPosition++;
				characterList_cursorPositionOnScreen++;
				if (characterList_cursorPosition > totalCharacters) {
					characterList_cursorPosition = totalCharacters;
					characterList_cursorPositionOnScreen = totalCharacters;
					if (totalCharacters-2 < 0) {
						characterShownFirst = 0;
					}
				} else if (characterList_cursorPosition > characterShownFirst+2) {
					characterShownFirst++;
				}
				if (characterList_cursorPositionOnScreen > 2) {
					characterList_cursorPositionOnScreen = 2;
				}
			}

			if ((hDown & KEY_DLEFT) && (highlightedGame > 1)) {
				sndHighlight();
				characterPage[highlightedGame]--;
				if (assistantChange) {
					if (characterPage[highlightedGame] < 1) characterPage[highlightedGame] = 1;
				} else {
					if (characterPage[highlightedGame] < 0) characterPage[highlightedGame] = 0;
				}
			}

			if ((hDown & KEY_DRIGHT) && (highlightedGame > 1)) {
				sndHighlight();
				characterPage[highlightedGame]++;
				if (assistantChange) {
					if (characterPage[highlightedGame] > 10) characterPage[highlightedGame] = 10;
				} else {
					if (characterPage[highlightedGame] > 11) characterPage[highlightedGame] = 11;
				}
			}

			if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
				characterList_cursorPosition = 0;
				characterList_cursorPositionOnScreen = 0;
				characterShownFirst = 0;
				getMaxChars();
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			if (assistantChange) {
				u16 charId = 0;
				switch (characterPage[3]) {
					case 0:
					default:
						charId = 0x0BAE;
						break;
					case 1:
						charId = ss4CharacterOrder_AtoB[characterList_cursorPosition];
						break;
					case 2:
						charId = ss4CharacterOrder_CtoD[characterList_cursorPosition];
						break;
					case 3:
						charId = ss4CharacterOrder_EtoF[characterList_cursorPosition];
						break;
					case 4:
						charId = ss4CharacterOrder_GtoI[characterList_cursorPosition];
						break;
					case 5:
						charId = ss4CharacterOrder_JtoL[characterList_cursorPosition];
						break;
					case 6:
						charId = ss4CharacterOrder_MtoN[characterList_cursorPosition];
						break;
					case 7:
						charId = ss4CharacterOrder_OtoP[characterList_cursorPosition];
						break;
					case 8:
						charId = ss4CharacterOrder_QtoS[characterList_cursorPosition];
						break;
					case 9:
						charId = ss4CharacterOrder_TtoV[characterList_cursorPosition];
						break;
					case 10:
						charId = ss4CharacterOrder_WtoZ[characterList_cursorPosition];
						break;
					case 11:
						charId = 0x0BB9+characterList_cursorPosition;
						break;
				}
				writeSS4AssistantCharacterId(charId);
				writeSS4AssistantCharacterToSave();
				assistantChange = false;
				characterPage[highlightedGame] = 0;
				characterList_cursorPosition = 1;
				characterList_cursorPositionOnScreen = 1;
				characterShownFirst = 0;
				getMaxChars();
			} else if (highlightedGame == 3 && characterList_cursorPosition == 1 && characterPage[highlightedGame] == 0) {
				assistantChange = true;
				characterPage[highlightedGame]++;
				characterList_cursorPosition = 0;
				characterList_cursorPositionOnScreen = 0;
				characterShownFirst = 0;
				getMaxChars();
			} else {
				subScreenMode = 1;
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			if (assistantChange) {
				assistantChange = false;
				characterPage[highlightedGame] = 0;
				characterList_cursorPosition = 1;
				characterList_cursorPositionOnScreen = 1;
				characterShownFirst = 0;
				getMaxChars();
			} else {
				characterList_cursorPosition = 0;
				characterList_cursorPositionOnScreen = 0;
				characterShownFirst = 0;
				characterChangeMenu_cursorPosition = 0;
				characterChangeMenu_cursorPositionOnScreen = 0;
				characterChangeMenu_optionShownFirst = 0;
				Gui::setScreen(std::make_unique<WhatToDo>(), true);
			}
		}

		if (hDown & KEY_START) {
			if (highlightedGame == 3) {
				sndSelect();
				subScreenMode = 7;
			} else if (highlightedGame == 2) {
				sndSelect();
				messageNo = ss3DLCharactersBackedUp ? 6 : 4;
				if (!ss3DLCharactersBackedUp) {
					peopleMetCount = false;
				}
				showMessage = true;
			}
		}
	}
}