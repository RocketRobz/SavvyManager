#include "emblemChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "savedata.h"
#include "file_browse.h"

#include "import_emblemnames.h"
#include <unistd.h>

EmblemChange::EmblemChange() {
	this->getMaxEmblems();
}

void EmblemChange::getMaxEmblems() {
	if (this->subScreenMode == 2) {
		if (this->importPage == 1) {
			this->totalEmblems = numberOfExportedEmblems-1;
		} else {
			this->totalEmblems = 10;
		}
	} else if (highlightedGame == 3) {
		this->totalEmblems = 2;
		readSS4Save();
		//readSS4Emblem(cursorPosition);
	} else {
		this->totalEmblems = 0;
		readSS3Save();
		//readSS3Emblem();
	}

	if (!this->modeInited) {
		//renderEmblem();
		this->modeInited = true;
	}
}

/*static u32 emblemPalette[16] = 
{
	C2D_Color32(0, 0, 0, 0),
	C2D_Color32(16, 16, 16, 255),
	C2D_Color32(32, 32, 32, 255),
	C2D_Color32(48, 48, 48, 255),
	C2D_Color32(64, 64, 64, 255),
	C2D_Color32(80, 80, 80, 255),
	C2D_Color32(96, 96, 96, 255),
	C2D_Color32(112, 112, 112, 255),
	C2D_Color32(128, 128, 128, 255),
	C2D_Color32(144, 144, 144, 255),
	C2D_Color32(160, 160, 160, 255),
	C2D_Color32(176, 176, 176, 255),
	C2D_Color32(192, 192, 192, 255),
	C2D_Color32(208, 208, 208, 255),
	C2D_Color32(224, 224, 224, 255),
	C2D_Color32(240, 240, 240, 255)
};

int EmblemChange::getPalNumber(u8 byte, bool secondPixel) {
	if (secondPixel) {
		if ((byte & 0x0F) == 0x0) {
			return 0;
		} else if ((byte & 0x0F) == 0x1) {
			return 1;
		} else if ((byte & 0x0F) == 0x2) {
			return 2;
		} else if ((byte & 0x0F) == 0x3) {
			return 3;
		} else if ((byte & 0x0F) == 0x4) {
			return 4;
		} else if ((byte & 0x0F) == 0x5) {
			return 5;
		} else if ((byte & 0x0F) == 0x6) {
			return 6;
		} else if ((byte & 0x0F) == 0x7) {
			return 7;
		} else if ((byte & 0x0F) == 0x8) {
			return 8;
		} else if ((byte & 0x0F) == 0x9) {
			return 9;
		} else if ((byte & 0x0F) == 0xA) {
			return 10;
		} else if ((byte & 0x0F) == 0xB) {
			return 11;
		} else if ((byte & 0x0F) == 0xC) {
			return 12;
		} else if ((byte & 0x0F) == 0xD) {
			return 13;
		} else if ((byte & 0x0F) == 0xE) {
			return 14;
		} else if ((byte & 0x0F) == 0xF) {
			return 15;
		}
	} else {
		if (byte >= 0x00 && byte < 0x10) {
			return 0;
		} else if (byte >= 0x10 && byte < 0x20) {
			return 1;
		} else if (byte >= 0x20 && byte < 0x30) {
			return 2;
		} else if (byte >= 0x30 && byte < 0x40) {
			return 3;
		} else if (byte >= 0x40 && byte < 0x50) {
			return 4;
		} else if (byte >= 0x50 && byte < 0x60) {
			return 5;
		} else if (byte >= 0x60 && byte < 0x70) {
			return 6;
		} else if (byte >= 0x70 && byte < 0x80) {
			return 7;
		} else if (byte >= 0x80 && byte < 0x90) {
			return 8;
		} else if (byte >= 0x90 && byte < 0xA0) {
			return 9;
		} else if (byte >= 0xA0 && byte < 0xB0) {
			return 10;
		} else if (byte >= 0xB0 && byte < 0xC0) {
			return 11;
		} else if (byte >= 0xC0 && byte < 0xD0) {
			return 12;
		} else if (byte >= 0xD0 && byte < 0xE0) {
			return 13;
		} else if (byte >= 0xE0 && byte < 0xF0) {
			return 14;
		} else if (byte >= 0xF0 && byte < 0x100) {
			return 15;
		}
	}
	return 0;
}

u32 EmblemChange::emblemPixel(int pixel, bool secondPixel) {
	pixel = pixel/2;
	return emblemPalette[getPalNumber(emblemData.sprite[pixel], secondPixel)];
}

void EmblemChange::renderEmblem(void) {
	bool secondPixel = false;
	for (int i = 0; i < 64*64; i++) {
		emblemImage[i] = emblemPixel(i, secondPixel);
		secondPixel = !secondPixel;
	}
}

void EmblemChange::drawEmblem(int x, int y, bool big) {
	for (int h = (emblemHalf ? 32: 0); h < (emblemHalf ? 64: 32); h++) {
		for (int w = 0; w < 64; w++) {
			Draw_Rect(x+(w*(big*2)), y+(h*(big*2)), 1+big, 1+big, emblemImage[(h*64)+w]);
		}
	}
	emblemHalf = !emblemHalf;
}*/

void EmblemChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	if (this->messageNo == 3) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Failed to import emblem.");
	} else if (this->messageNo == 2) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Emblem exported successfully.");
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "You can go to \"Import Emblems\"");
		Gui::DrawStringCentered(0, 114, 0.60, BLACK, "and restore the exported emblem");
		Gui::DrawStringCentered(0, 134, 0.60, BLACK, "at any time.");
	} else if (this->messageNo == 1) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, emblemImported);
	} else {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "This feature is not available yet.");
		//Gui::DrawStringCentered(0, 104, 0.60, BLACK, "yet.");
	}
	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void EmblemChange::Draw(void) const {
	Gui::ScreenDraw(Top);

	Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
		}
	}
	GFX::DrawSprite(sprites_emblem_back_idx, 100, 20, 2, 2);
	//drawEmblem(136, 56, true);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			GFX::DrawSprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	char emblemText[32];
	this->cursorX = 248;
	if (this->subScreenMode == 2) {
		this->cursorY = 64+(48*this->importEmblemList_cursorPositionOnScreen);

		// Game name
		switch (this->importPage) {
			case 1:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, "Your emblem files");
				break;
			case 0:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, "Savvy Manager");
				break;
		}
		Gui::DrawString(8, 8, 0.50, BLACK, "<");
		Gui::DrawString(304, 8, 0.50, BLACK, ">");

	  if (!this->displayNothing) {
		int i2 = 48;
		for (int i = this->import_emblemShownFirst; i < this->import_emblemShownFirst+3; i++) {
			if (this->importPage == 1) {
				if (i >= numberOfExportedEmblems) break;
			} else {
				if (i > this->totalEmblems) break;
			}
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (importPage == 1) {
				Gui::DrawString(32, i2, 0.65, BLACK, getExportedEmblemName(i));
			} else {
				Gui::DrawString(32, i2, 0.65, BLACK, import_emblemNames[i]);
			}
			i2 += 48;
		}
	  }
	} else if (this->subScreenMode == 1) {
		if (highlightedGame == 2) {
			sprintf(emblemText, "Emblem");
		} else {
			sprintf(emblemText, "Emblem %i", this->cursorPosition+1);
		}

		this->cursorY = 64+(48*this->emblemChangeMenu_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, emblemText);

		int i2 = 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Import emblem");
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Export emblem");
	} else {
		this->cursorY = 64+(48*this->cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the emblem to change.");

		int i2 = 48;
		for (int i = 0; i <= this->totalEmblems; i++) {
			GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
			if (highlightedGame == 2) {
				sprintf(emblemText, "Emblem");
			} else {
				sprintf(emblemText, "Emblem %i", i+1);
			}
			Gui::DrawString(32, i2, 0.65, BLACK, emblemText);
			i2 += 48;
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


void EmblemChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			this->showMessage = false;
		}
	} else {
		if (this->subScreenMode == 2) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					this->importEmblemList_cursorPosition--;
					this->importEmblemList_cursorPositionOnScreen--;
					if (this->importEmblemList_cursorPosition < 0) {
						this->importEmblemList_cursorPosition = 0;
						this->import_emblemShownFirst = 0;
					} else if (this->importEmblemList_cursorPosition < this->import_emblemShownFirst) {
						this->import_emblemShownFirst--;
					}

					if (this->importEmblemList_cursorPositionOnScreen < 0) {
						this->importEmblemList_cursorPositionOnScreen = 0;
					}
				}

				if (hDown & KEY_DOWN) {
					sndHighlight();
					this->importEmblemList_cursorPosition++;
					this->importEmblemList_cursorPositionOnScreen++;
					if (this->importEmblemList_cursorPosition > this->totalEmblems) {
						this->importEmblemList_cursorPosition = this->totalEmblems;
						this->import_emblemShownFirst = this->totalEmblems-2;
						if (this->import_emblemShownFirst < 0) this->import_emblemShownFirst = 0;
						if (this->importEmblemList_cursorPositionOnScreen > this->totalEmblems) {
							this->importEmblemList_cursorPositionOnScreen = this->totalEmblems;
						}
					} else if (this->importEmblemList_cursorPosition > this->import_emblemShownFirst+2) {
						this->import_emblemShownFirst++;
					}

					if (this->importEmblemList_cursorPositionOnScreen > 2) {
						this->importEmblemList_cursorPositionOnScreen = 2;
					}
				}
			}

			if (hDown & KEY_A) {
				bool exportFound = false;
				if (this->importPage == 1 && this->totalEmblems > 0) {
					switch (highlightedGame) {
						case 3:
							sprintf(this->embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(this->importEmblemList_cursorPosition));
							if (access(this->embFilePath, F_OK) == 0) {
								sndSelect();
								readSS4EmblemFile(this->cursorPosition, this->embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(this->embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(this->importEmblemList_cursorPosition));
							if (access(this->embFilePath, F_OK) == 0) {
								sndSelect();
								readSS3EmblemFile(this->embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(this->emblemImported, "Imported %s successfully.", getExportedEmblemName(this->importEmblemList_cursorPosition));
						this->messageNo = 1;
						this->subScreenMode = 1;
					} else {
						sndBack();
						this->messageNo = 3;
					}
					this->showMessage = true;
					} else if (importPage == 0) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(this->embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[this->importEmblemList_cursorPosition]);
							if (access(this->embFilePath, F_OK) == 0) {
								readSS4EmblemFile(this->cursorPosition, this->embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(this->embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[this->importEmblemList_cursorPosition]);
							if (access(this->embFilePath, F_OK) == 0) {
								readSS3EmblemFile(this->embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(emblemImported, "Imported %s successfully.", import_emblemNames[this->importEmblemList_cursorPosition]);
						this->messageNo = 1;
						this->subScreenMode = 1;
					} else {
						sndBack();
						this->messageNo = 3;
					}
					this->showMessage = true;
				}
			}

			if (hDown & KEY_LEFT) {
				sndHighlight();
				this->importPage--;
				if (this->importPage < 0) this->importPage = 1;
				this->importEmblemList_cursorPosition = 0;
				this->importEmblemList_cursorPositionOnScreen = 0;
				this->import_emblemShownFirst = 0;
				this->getMaxEmblems();
			}

			if (hDown & KEY_RIGHT) {
				sndHighlight();
				this->importPage++;
				if (this->importPage > 1) this->importPage = 0;
				this->importEmblemList_cursorPosition = 0;
				this->importEmblemList_cursorPositionOnScreen = 0;
				this->import_emblemShownFirst = 0;
				this->getMaxEmblems();
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				this->subScreenMode = 1;
			}
		} else if (this->subScreenMode == 1) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					this->emblemChangeMenu_cursorPosition--;
					if (this->emblemChangeMenu_cursorPosition < 0) {
						this->emblemChangeMenu_cursorPosition = 0;
					}
				}

				if (hDown & KEY_DOWN) {
					sndHighlight();
					this->emblemChangeMenu_cursorPosition++;
					if (this->emblemChangeMenu_cursorPosition > 1) {
						this->emblemChangeMenu_cursorPosition = 1;
					}
				}
			}

			if (hDown & KEY_A) {
				if (this->emblemChangeMenu_cursorPosition == 1) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(this->embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem %i.chr", this->cursorPosition);
							writeSS4EmblemFile(this->cursorPosition, this->embFilePath);
							break;
						case 2:
							sprintf(this->embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem.chr");
							writeSS3EmblemFile(this->embFilePath);
							break;
					}
					this->messageNo = 2;
					this->showMessage = true;
				} else {
					sndSelect();
					this->subScreenMode = 2;
					this->displayNothing = true;
					gspWaitForVBlank();
					getExportedEmblemContents();
					this->getMaxEmblems();
					this->displayNothing = false;
				}
			}
			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				this->subScreenMode = 0;
				this->getMaxEmblems();
			}
		} else {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					this->cursorPosition--;
					if (this->cursorPosition < 0) {
						this->cursorPosition = 0;
					}
					this->modeInited = false;
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					this->cursorPosition++;
					if (this->cursorPosition > this->totalEmblems) {
						this->cursorPosition = this->totalEmblems;
					}
					this->modeInited = false;
				}

				if (hDown & KEY_A) {
					sndSelect();
					this->subScreenMode = 1;
				}
				if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
					sndBack();
					Gui::setScreen(std::make_unique<WhatToDo>(), true);
				}
			}
		}
	}
}