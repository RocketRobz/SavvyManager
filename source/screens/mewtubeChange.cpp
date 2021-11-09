#include "mewtubeChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "danceCpk.hpp"
#include "savedata.h"

#include "ss4charnames.h"

static const char* stageTitle[] = {
	"Open Plaza",
	"Open Plaza",
	"Outdoor Festival",
	"Outdoor Festival",
	"Concert Hall",
	"Concert Hall",
	"Charity Stage",
	"Charity Stage",
	"Live Music Club",
	"Live Music Club",
	"Celestial Hotel",
	"Celestial Hotel",
	"Alinatron On-Air",
	"Promo Video",
	"Cinema",
	"Concert Hall",
	"Concert Hall",
	"Open Plaza",
	"Open Plaza",
};

static const char* songTitle[] = {
	"Flying Shiny Day",
	"Ready Go!!",
	"Flying Shiny Day",
	"Ready Go!!",
	"Higher Higher",
	"To the Future",
	"Higher Higher",
	"To the Future",
	"Wings",
	"Beautiful",
	"Make Me Glow",
	"Glowing Moon",
	"Fight For Your Style",
	"Twinkle Fantasia",
	"Maria",
	"Dosukoi Koi Koi (Break It Down)",
	"Radiance",
	"Ring a Ding",
	"Bravo",
};

extern const char* getSS4CharName(u16 charId);

MewtubeChange::MewtubeChange() {
	peopleMet = 0;
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
}

void MewtubeChange::getMaxChars() {
	switch (characterPage) {
		case 0:
		default:
			totalCharacters = 0;
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
}

void MewtubeChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_question_idx, 132, -2);

	Gui::DrawStringCentered(0, 58, 0.60, BLACK, "The character will be reverted to");
	Gui::DrawStringCentered(0, 78, 0.60, BLACK, "the original one from the video.");
	Gui::DrawStringCentered(0, 104, 0.60, BLACK, "Her outfit, hairstyle, makeup,");
	Gui::DrawStringCentered(0, 124, 0.60, BLACK, "and nails will be included.");
	Gui::DrawStringCentered(0, 144, 0.60, BLACK, "Is this OK?");

	GFX::DrawSprite(sprites_button_msg_shadow_idx, 52, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 53, 188);
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 176, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 177, 188);
	Gui::DrawString(72, 196, 0.70, MSG_BUTTONTEXT, " No");
	Gui::DrawString(196, 196, 0.70, MSG_BUTTONTEXT, " Yes");
}

void MewtubeChange::Draw(void) const {
	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(255, 239, 123, 255));
	bool light = true;
	int x = 0;
	int width = 29;
	for (int i = 0; i < 14; i++) {
		if (light) {
			Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(255, 247, 173, 255));
		}
		light = !light;
		x += width;
		width = light ? 29 : 28;
	}
	if (subScreenMode == 2 && peopleMet > 0) {
		Gui::Draw_Rect(0, 208, 400, 32, WHITE);
		char peopleMetText[24];
		sprintf(peopleMetText, "People Met: %i", peopleMet);
		Gui::DrawStringCentered(0, 216, 0.60, BLACK, peopleMetText);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(255, 251, 189, 255));
	bool dark = true;
	x = 0;
	width = 17;
	for (int i = 0; i < 14; i++) {
		if (dark) {
			Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(245, 239, 140, 255));
		}
		dark = !dark;
		x += width;
		width = dark ? 29 : 28;
	}
	Gui::Draw_Rect(0, 0, 320, 33, C2D_Color32(247, 235, 206, 255));
	Gui::Draw_Rect(0, 33, 320, 5, C2D_Color32(214, 158, 58, 255));

	cursorX = 248;
	cursorY = 64+(48*cursorPositionOnScreen[subScreenMode]);

	int i2 = 56;
	if (subScreenMode == 2) {
		const char* letterText[] =    { "A", "C", "E", "G", "J", "M", "O", "Q", "T", "W"};
		const char* letterTextBot[] = {"-B","-D","-F","-I","-L","-N","-P","-S","-V","-Z"};
		Gui::DrawString(8, 10, 0.50, characterPage==0 ? RED : BLACK, "Main");
		for (int i = 0; i < 10; i++) {
			Gui::DrawString(40+(i*24), 4, 0.50, (characterPage > 0 && characterPage-1 == i) ? RED : BLACK, letterText[i]);
			Gui::DrawString(48+(i*24), 16, 0.50, (characterPage > 0 && characterPage-1 == i) ? RED : BLACK, letterTextBot[i]);
		}
		Gui::DrawString(292, 10, 0.50, characterPage==11 ? RED : BLACK, "Ext.");

		char chrCounter[24];
		sprintf(chrCounter, "%d/%d", cursorPosition[2]+1, totalCharacters+1);
		Gui::DrawString(64, 184, 0.55, BLACK, chrCounter);

		for (int i = characterShownFirst; i < characterShownFirst+3; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (characterPage == 0) {
				GFX::DrawSprite((getSS4CharacterGender(0xBAE) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, BLACK, ss4PlayerName);
				break;
			} else {
				u16 charId = 0;
				switch (characterPage) {
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
			i2 += 48;
		}
		textTimer = 0;
	} else if (subScreenMode == 1) {
		Gui::DrawString(8, 8, 0.50, BLACK, "Select the character you want to change.");
		for (int i = 0; i < danceGetPeopleAmount(cursorPosition[0]); i++) {
			u16 charId = getSS4MewtubeCharacterId(cursorPosition[0], 3+i);
			u16 orgCharId = getSS4MewtubeCharacterId(cursorPosition[0], i);
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (charId != 0) {
				GFX::DrawSprite((getSS4CharacterGender(charId) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
			}
			if (charId == orgCharId) {
				Gui::DrawString(64, i2, 0.65, BLACK, getSS4CharName(charId));
			} else {
				Gui::DrawString(64, i2-6, 0.60, BLACK, getSS4CharName(charId));
				Gui::DrawString(64, i2+12, 0.50, BLACK, getSS4CharName(orgCharId));
			}
			i2 += 48;
		}
		u16 charId = getSS4MewtubeCharacterId(cursorPosition[0], 3+cursorPosition[1]);
		if (cursorPosition[0] != 12 && textTimer > iFps*3) {
			Gui::DrawString(88, 201, 0.50, BLACK,"LEFT/RIGHT: Remove/Add");
		} else if (charId != 0) {
			Gui::DrawString(88, 201, 0.50, BLACK, ": Blank out");
		}
		textTimer++;
		if (textTimer > iFps*6 || charId == 0) textTimer = 0;
		Gui::DrawString(88, 217, 0.50, BLACK, "SELECT: Revert to original");
	} else {
		char chrCounter[24];
		sprintf(chrCounter, "%d/%d", cursorPosition[0]+1, numberofVideos+1);
		Gui::DrawString(64, 184, 0.55, BLACK, chrCounter);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the video to change in You Diva mode.");
		for (int i = videoShownFirst; i < videoShownFirst+3; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			Gui::DrawString(32, i2-8, 0.50, BLACK, stageTitle[i]);
			Gui::DrawString(32, i2+8, 0.60, BLACK, songTitle[i]);
			i2 += 48;
		}
		textTimer = 0;
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


void MewtubeChange::Logic(u32 hDown, u32 hDownRepeat, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 176 && touch.px < 176+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			revertSS4MewtubeCharacter(cursorPosition[0], 3+cursorPosition[1]);
			writeSS4Save();
			showMessage = false;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touch.px >= 52 && touch.px < 52+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndBack();
			showMessage = false;
		}
	} else if (subScreenMode == 2) {
		if (showCursor) {
			if ((hDownRepeat & KEY_DUP) && (characterPage > 0)) {
				sndHighlight();
				cursorPosition[2]--;
				cursorPositionOnScreen[2]--;
				if (cursorPosition[2] < 0) {
					cursorPosition[2] = 0;
					characterShownFirst = 0;
				} else if (cursorPosition[2] < characterShownFirst) {
					characterShownFirst--;
				}
				if (cursorPositionOnScreen[2] < 0) {
					cursorPositionOnScreen[2] = 0;
				}
				getMaxChars();
			}

			if ((hDownRepeat & KEY_DDOWN) && (characterPage > 0)) {
				sndHighlight();
				cursorPosition[2]++;
				cursorPositionOnScreen[2]++;
				if (cursorPosition[2] > totalCharacters) {
					cursorPosition[2] = totalCharacters;
					characterShownFirst = totalCharacters-2;
				} else if (cursorPosition[2] > characterShownFirst+2) {
					characterShownFirst++;
				}
				if (cursorPositionOnScreen[2] > 2) {
					cursorPositionOnScreen[2] = 2;
				}
			}

			if ((hDown & KEY_DLEFT) && (highlightedGame > 1)) {
				sndHighlight();
				characterPage--;
				if (characterPage < 0) characterPage = 0;
			}

			if ((hDown & KEY_DRIGHT) && (highlightedGame > 1)) {
				sndHighlight();
				characterPage++;
				if (characterPage > 11) characterPage = 11;
			}

			if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
				cursorPosition[2] = 0;
				cursorPositionOnScreen[2] = 0;
				characterShownFirst = 0;
				getMaxChars();
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			u16 charId = 0;
			switch (characterPage) {
				case 0:
				default:
					charId = 0x0BAE;
					break;
				case 1:
					charId = ss4CharacterOrder_AtoB[cursorPosition[2]];
					break;
				case 2:
					charId = ss4CharacterOrder_CtoD[cursorPosition[2]];
					break;
				case 3:
					charId = ss4CharacterOrder_EtoF[cursorPosition[2]];
					break;
				case 4:
					charId = ss4CharacterOrder_GtoI[cursorPosition[2]];
					break;
				case 5:
					charId = ss4CharacterOrder_JtoL[cursorPosition[2]];
					break;
				case 6:
					charId = ss4CharacterOrder_MtoN[cursorPosition[2]];
					break;
				case 7:
					charId = ss4CharacterOrder_OtoP[cursorPosition[2]];
					break;
				case 8:
					charId = ss4CharacterOrder_QtoS[cursorPosition[2]];
					break;
				case 9:
					charId = ss4CharacterOrder_TtoV[cursorPosition[2]];
					break;
				case 10:
					charId = ss4CharacterOrder_WtoZ[cursorPosition[2]];
					break;
				case 11:
					charId = 0x0BB9+cursorPosition[2];
					break;
			}
			readSS4Character(charId);
			writeSS4MewtubeCharacterId(charId, cursorPosition[0], 3+cursorPosition[1]);
			writeSS4MewtubeCharacter(cursorPosition[0], 3+cursorPosition[1]);
			writeSS4MewtubeCharacterToSave(cursorPosition[0], 3+cursorPosition[1]);	// Write to save data
			subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			subScreenMode = 1;
		}
	} else if (subScreenMode == 1) {
		if (showCursor) {
			if (hDown & KEY_UP) {
				sndHighlight();
				cursorPosition[1]--;
				cursorPositionOnScreen[1]--;
				if (cursorPosition[1] < 0) {
					cursorPosition[1] = 0;
				}
				if (cursorPositionOnScreen[1] < 0) {
					cursorPositionOnScreen[1] = 0;
				}
			}
			if (hDown & KEY_DOWN) {
				sndHighlight();
				cursorPosition[1]++;
				cursorPositionOnScreen[1]++;
				if (cursorPosition[1] > danceGetPeopleAmount(cursorPosition[0])-1) {
					cursorPosition[1] = danceGetPeopleAmount(cursorPosition[0])-1;
				}
				if (cursorPositionOnScreen[1] > danceGetPeopleAmount(cursorPosition[0])-1) {
					cursorPositionOnScreen[1] = danceGetPeopleAmount(cursorPosition[0])-1;
				}
			}
			if ((hDown & KEY_LEFT) && cursorPosition[0] != 12) {
				int peopleAmount = danceGetPeopleAmount(cursorPosition[0]);
				peopleAmount--;
				if (peopleAmount < 1) {
					peopleAmount = 1;
				} else {
					sndHighlight();
					danceSetPeopleAmount(cursorPosition[0], peopleAmount);
					danceWriteDataToCpk(cursorPosition[0]);
					if (getSS4MewtubeCharacterId(cursorPosition[0], peopleAmount) != 0) {
						writeSS4MewtubeCharacterId(0, cursorPosition[0], peopleAmount);
						writeSS4MewtubeCharacter(cursorPosition[0], peopleAmount);
						writeSS4MewtubeCharacterToSave(cursorPosition[0], peopleAmount);	// Write to save data
					}
				}
			}
			if ((hDown & KEY_RIGHT) && cursorPosition[0] != 12) {
				int peopleAmount = danceGetPeopleAmount(cursorPosition[0]);
				peopleAmount++;
				if (peopleAmount > 3) {
					peopleAmount = 3;
				} else {
					sndHighlight();
					danceSetPeopleAmount(cursorPosition[0], peopleAmount);
					danceWriteDataToCpk(cursorPosition[0]);
					if (getSS4MewtubeCharacterId(cursorPosition[0], peopleAmount-1) == 0) {
						u16 charId = danceGetCharacterId(cursorPosition[0], peopleAmount-1);
						readSS4Character(charId);
						writeSS4MewtubeCharacterId(charId, cursorPosition[0], peopleAmount-1);
						writeSS4MewtubeCharacter(cursorPosition[0], peopleAmount-1);
						writeSS4MewtubeCharacterToSave(cursorPosition[0], peopleAmount-1);	// Write to save data
					}
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			subScreenMode = 2;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			cursorPosition[1] = 0;
			cursorPositionOnScreen[1] = 0;
			subScreenMode = 0;
		}

		if (hDown & KEY_X) {
			u16 charId = getSS4MewtubeCharacterId(cursorPosition[0], 3+cursorPosition[1]);
			if (charId != 0) {
				sndSelect();
				writeSS4MewtubeCharacterId(0, cursorPosition[0], 3+cursorPosition[1]);
				writeSS4MewtubeCharacterToSave(cursorPosition[0], 3+cursorPosition[1]);	// Write to save data
			}
		}

		if (hDown & KEY_SELECT) {
			sndSelect();
			showMessage = true;
		}
	} else {
		if (showCursor) {
			if (hDownRepeat & KEY_UP) {
				sndHighlight();
				cursorPosition[0]--;
				cursorPositionOnScreen[0]--;
				if (cursorPosition[0] < 0) {
					cursorPosition[0] = 0;
					videoShownFirst = 0;
				} else if (cursorPosition[0] < videoShownFirst) {
					videoShownFirst--;
				}
				if (cursorPositionOnScreen[0] < 0) {
					cursorPositionOnScreen[0] = 0;
				}
			}
			if (hDownRepeat & KEY_DOWN) {
				sndHighlight();
				cursorPosition[0]++;
				cursorPositionOnScreen[0]++;
				if (cursorPosition[0] > numberofVideos) {
					cursorPosition[0] = numberofVideos;
					videoShownFirst = numberofVideos-2;
					if (videoShownFirst < 0) videoShownFirst = 0;
					if (cursorPositionOnScreen[0] > numberofVideos) {
						cursorPositionOnScreen[0] = numberofVideos;
					}
				} else if (cursorPosition[0] > videoShownFirst+2) {
					videoShownFirst++;
				}
				if (cursorPositionOnScreen[0] > 2) {
					cursorPositionOnScreen[0] = 2;
				}
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			subScreenMode = 1;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			Gui::setScreen(std::make_unique<WhatToDo>(), true);
		}
	}
}