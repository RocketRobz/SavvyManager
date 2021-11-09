#include <3ds.h>
#include <stdio.h>
#include <unistd.h>		// access
#include "tonccpy.h"

extern u8 saveRegion[4];

static const char* danceCpkPath = "sdmc:/luma/titles/00040000001C2500/romfs/USA/Common/Scene/dance.cpk";
char danceCpkData[0x21188];

void readDanceCpk(void) {
	switch (saveRegion[3]) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			danceCpkPath = "sdmc:/luma/titles/00040000001C2600/romfs/EUR/Common/Scene/dance.cpk";
			break;
		case CFG_REGION_JPN:
			danceCpkPath = "sdmc:/luma/titles/000400000019F600/romfs/JPN/Common/Scene/dance.cpk";
			break;
	}

	if (access(danceCpkPath, F_OK) != 0) {
		FILE* file = fopen((saveRegion[3] == CFG_REGION_JPN) ? "romfs:/danceJ.cpk" : "romfs:/dance.cpk", "rb");
		fread(danceCpkData, 1, (int)sizeof(danceCpkData), file);
		fclose(file);

		file = fopen(danceCpkPath, "wb");
		fwrite(danceCpkData, 1, (saveRegion[3] == CFG_REGION_JPN) ? 0x19910 : 0x21188, file);
		fclose(file);

		return;
	}

	FILE* file = fopen(danceCpkPath, "rb");
	fread(danceCpkData, 1, (int)sizeof(danceCpkData), file);
	fclose(file);
}

static inline void writeDanceCpkDword(const u16* dst, u16 dword) {
	*(u16*)(danceCpkData + (u32)dst) = dword;
}

static inline void writeDanceCpkWord(const u32* dst, u32 word) {
	*(u32*)(danceCpkData + (u32)dst) = word;
}

int danceGetPeopleAmount(const int video) {
	if (saveRegion[3] == CFG_REGION_JPN) {
		return (int)danceCpkData[0x800 + 2 + 0xAC*video];
	}
	return (int)danceCpkData[0x800 + 2 + 0xB0*video];
}

void danceSetPeopleAmount(const int video, int amount) {
	if (video == 12) {
		return;
	}

	if (amount < 1) amount = 1;
	if (amount > 3) amount = 3;

	const u16* charIdLoc = (u16*)((u32)0x800 + 0x64 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video)));
	const u16* charAniLoc = (u16*)((u32)0x800 + 0x6A + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video)));
	const u32* groupFormLoc = (u32*)((u32)0x800 + 0x70 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video)));
	u32 charOrderLoc = 0x800 + 0xA8 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video));

	danceCpkData[0x800 + 2 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video))] = amount;
	amount--;

	switch (amount) {
		case 0:
		case 1:
		default:
			danceCpkData[charOrderLoc] = 1;
			danceCpkData[charOrderLoc+1] = (amount == 1) ? 2 : 0;
			danceCpkData[charOrderLoc+2] = 0;
			break;
		case 2:
			danceCpkData[charOrderLoc] = 2;
			danceCpkData[charOrderLoc+1] = 1;
			danceCpkData[charOrderLoc+2] = 3;
			break;
	}

	switch (video) {
		case 0: // Open Plaza: Flying Shiny Day
		default:
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05C5);
				writeDanceCpkDword(charAniLoc+2, 0x05C6);
				writeDanceCpkWord(groupFormLoc, 0xA194082A);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				writeDanceCpkDword(charAniLoc+1, 0x05AA);
				writeDanceCpkDword(charAniLoc+2, 0x05AB);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkWord(groupFormLoc, 0x9C67D93E);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 1: // Open Plaza: Ready Go!!
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05AE);
				writeDanceCpkDword(charAniLoc+2, 0x05AF);
				writeDanceCpkWord(groupFormLoc, 0xA1940829);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				writeDanceCpkDword(charAniLoc+1, 0x057C);
				writeDanceCpkDword(charAniLoc+2, 0x057C);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkWord(groupFormLoc, 0x9C67D93E);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 2: // Outdoor Festival: Flying Shiny Day
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05C5);
				writeDanceCpkDword(charAniLoc+2, 0x05C6);
				writeDanceCpkWord(groupFormLoc, 0x9C67D939);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				writeDanceCpkDword(charAniLoc+1, 0x05AA);
				writeDanceCpkDword(charAniLoc+2, 0x05AB);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkWord(groupFormLoc, 0x9C67D93E);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 3: // Outdoor Festival: Ready Go!!
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05AE);
				writeDanceCpkDword(charAniLoc+2, 0x05AF);
				writeDanceCpkWord(groupFormLoc, 0x9C67D939);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				writeDanceCpkDword(charAniLoc+1, 0x057C);
				writeDanceCpkDword(charAniLoc+2, 0x057C);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkWord(groupFormLoc, 0x9C67D93E);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 4: // Concert Hall: Higher Higher
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0xFE21E3D1);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D2);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 5: // Concert Hall: To the Future
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc, 0x057E);
				writeDanceCpkDword(charAniLoc+1, 0x05B2);
				writeDanceCpkDword(charAniLoc+2, 0x05B3);
				writeDanceCpkWord(groupFormLoc, 0xFE21E3D1);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x43);
				writeDanceCpkDword(charAniLoc+1, 0x05B4);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x44);
					writeDanceCpkDword(charAniLoc+2, 0x05B4);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D2);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc, 0x05C4);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 6: // Charity Stage: Higher Higher
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc, 0x057D);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0xFE21E3D1);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x06);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x07);
					writeDanceCpkDword(charAniLoc, 0x057D);
					writeDanceCpkDword(charAniLoc+1, 0x05B0);
					writeDanceCpkDword(charAniLoc+2, 0x05B1);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D2);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc, 0x05C2);
					writeDanceCpkDword(charAniLoc+1, 0x05C3);
					writeDanceCpkDword(charAniLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 7: // Charity Stage: To the Future
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc, 0x057E);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0xFE21E3D1);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x06);
				writeDanceCpkDword(charAniLoc+1, 0x05B4);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x07);
					writeDanceCpkDword(charAniLoc, 0x057E);
					writeDanceCpkDword(charAniLoc+1, 0x05B4);
					writeDanceCpkDword(charAniLoc+2, 0x05B4);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D2);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc, 0x05C4);
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3DF);
				}
			}
			break;
		case 8: // Live Music Club: Wings
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05AC);
				writeDanceCpkDword(charAniLoc+2, 0x05AD);
				writeDanceCpkWord(groupFormLoc, 0xBFA664C4);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x52);
				writeDanceCpkDword(charAniLoc+1, 0x057A);
				writeDanceCpkDword(charAniLoc+2, 0x057A);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x53);
					writeDanceCpkWord(groupFormLoc, 0x3E89C708);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 9: // Live Music Club: Beautiful
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0x05DD);
				writeDanceCpkDword(charIdLoc+2, 0x05DC);
				writeDanceCpkDword(charAniLoc+1, 0x05AC);
				writeDanceCpkDword(charAniLoc+2, 0x05AD);
				writeDanceCpkWord(groupFormLoc, 0xBFA664C4);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x52);
				writeDanceCpkDword(charAniLoc+1, 0x05B5);
				writeDanceCpkDword(charAniLoc+2, 0x05B6);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x53);
					writeDanceCpkWord(groupFormLoc, 0x3E89C708);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 10: // Celestial Hotel: Make Me Glow
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0xE3C262CE);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x52);
				writeDanceCpkDword(charAniLoc+1, 0x05BD);
				writeDanceCpkDword(charAniLoc+2, 0x05BE);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x53);
					writeDanceCpkWord(groupFormLoc, 0xE3C262CF);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 11: // Celestial Hotel: Glowing Moon
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0xE3C262CE);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x52);
				writeDanceCpkDword(charAniLoc+1, 0x0581);
				writeDanceCpkDword(charAniLoc+2, 0x0581);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x53);
					writeDanceCpkWord(groupFormLoc, 0xE3C262CF);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 12: // Alinatron On-Air: Fight For Your Style
			break;
		case 13: // Promo Video: Twinkle Fantasia
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0x6DC190BE);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x54);
				writeDanceCpkDword(charAniLoc+1, 0x057B);
				writeDanceCpkDword(charAniLoc+2, 0x057B);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x55);
					writeDanceCpkWord(groupFormLoc, 0x7D054615);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 14: // Cinema: Maria
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0);
				writeDanceCpkDword(charAniLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0x7D054614);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x54);
				writeDanceCpkDword(charAniLoc+1, 0x057B);
				writeDanceCpkDword(charAniLoc+2, 0x057B);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x55);
					writeDanceCpkWord(groupFormLoc, 0x7D054615);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 15: // Concert Hall: Dosukoi Koi Koi (Break It Down)
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0x7D054614);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x54);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x55);
					writeDanceCpkWord(groupFormLoc, 0x7D054615);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 16: // Concert Hall: Radiance
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkWord(groupFormLoc, 0x7D054614);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x54);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x55);
					writeDanceCpkWord(groupFormLoc, 0x7D05461A);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 17: // Open Plaza: Ring a Ding
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0x05C0);
				writeDanceCpkWord(groupFormLoc, 0xA194082A);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x02);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x03);
					writeDanceCpkDword(charAniLoc+1, 0x05C0);
					writeDanceCpkWord(groupFormLoc, 0x20776A6F);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc+1, 0x05BF);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
		case 18: // Open Plaza: Bravo
			if (amount == 0) {
				writeDanceCpkDword(charIdLoc+1, 0);
				writeDanceCpkDword(charIdLoc+2, 0);
				writeDanceCpkDword(charAniLoc+1, 0x0589);
				writeDanceCpkWord(groupFormLoc, 0xA194082A);
			} else {
				writeDanceCpkDword(charIdLoc+1, 0x01);
				if (amount == 2) {
					writeDanceCpkDword(charIdLoc+2, 0x03);
					writeDanceCpkDword(charAniLoc+1, 0x0589);
					writeDanceCpkWord(groupFormLoc, 0x9F5ACCA1);
				} else {
					writeDanceCpkDword(charIdLoc+2, 0);
					writeDanceCpkDword(charAniLoc+1, 0x0588);
					writeDanceCpkWord(groupFormLoc, 0xFE21E3D0);
				}
			}
			break;
	}
}

u16 danceGetCharacterId(const int video, const int slot) {
	return *(u16*)((u32)danceCpkData + 0x800 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video)) + 0x64 + slot*sizeof(u16));
}

void danceWriteDataToCpk(const int video) {
	u32 charIdLoc = 0x800 + (saveRegion[3] == CFG_REGION_JPN ? (0xAC*video) : (0xB0*video));

	FILE* file = fopen(danceCpkPath, "rb+");
	fseek(file, charIdLoc, SEEK_SET);
	fwrite(danceCpkData + charIdLoc, 1, 0xAC, file);
	fclose(file);
}