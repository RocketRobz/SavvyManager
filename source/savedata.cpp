#include <3ds.h>
#include <stdio.h>
#include <cstring>
#include "savedata.h"
#include "stringtool.h"
#include "tonccpy.h"

u8 saveRegion[4] = {0};

ss2character ss2CharacterData;
ss3to4character ss4CharacterData;

ss3to4emblem emblemData;

const char* ss1SavePath;

char ss1Save[0x100000];
char ss2Save[0x31736];
char ss3Save[0x174000];
char ss4Save[0xF0000];

char ss1PlayerName[10] = {0};
char ss2PlayerName[10] = {0};
char ss3PlayerName[10] = {0};
char ss4PlayerName[10] = {0};
char dlCharName[16] = {0};

static bool ss1SaveRead = false;
static bool ss2SaveRead = false;
static bool ss3SaveRead = false;
static bool ss4SaveRead = false;

static bool ss2SaveModified = false;
static bool ss3SaveModified = false;
//static bool ss4SaveModified = false;

Handle handle4;
FS_Archive archive4;

void commitSaveData(void) {
	if (ss2SaveModified) {
		archiveCommitSaveData("ss2");
	}
	if (ss3SaveModified) {
		archiveCommitSaveData("ss3");
	}
}

/*
	Style Savvy
*/
void readSS1Save(void) {
	if (ss1SaveRead) return;

	FILE* saveData = fopen(ss1SavePath, "rb");
	fread(ss1Save, (int)sizeof(ss1Save), 1, saveData);
	fclose(saveData);

	// Get playable character's name
	if (ss1Save[0x204] == 0x86 || ss1Save[0x204] == 0x87) {
		for (int i = 0; i < 9; i++) {
			ss1PlayerName[i] = ss1Save[0x3AB8+(i)];
		}
	} else {
		for (int i = 0; i < 9; i++) {
			ss1PlayerName[i] = ss1Save[0x63B8+(i)];
		}
	}

	ss2SaveRead = true;
}

void writeSS1Save(void) {
	remove(ss1SavePath);
	FILE* saveData = fopen(ss1SavePath, "wb");
	fwrite(ss1Save, (int)sizeof(ss1Save), 1, saveData);
	fclose(saveData);
}

/*
	Style Savvy: Trendsetters
*/
void readSS2Save(void) {
	if (ss2SaveRead) return;

	FILE* saveData = fopen(ss2SavePath, "rb");
	fread(ss2Save, (int)sizeof(ss2Save), 1, saveData);
	fclose(saveData);

	// Get playable character's name
	for (int i = 0; i < 9; i++) {
		ss2PlayerName[i] = ss2Save[0x1AC8+(i*2)];
	}

	ss2SaveRead = true;
}

void writeSS2Save(void) {
	FILE* saveData = fopen(ss2SavePath, "wb");
	fwrite(ss2Save, (int)sizeof(ss2Save), 1, saveData);
	fclose(saveData);

	ss2SaveModified = true;
}

void readSS2Character(void) {
	// Playable character
	tonccpy(&ss2CharacterData, (char*)ss2Save+(0x102), 0x3);
	tonccpy((char*)&ss2CharacterData+0x3, (char*)ss2Save+(0x106), 0x1);
	tonccpy((char*)&ss2CharacterData+0x4, (char*)ss2Save+(0x130), 0x18);
	tonccpy((char*)&ss2CharacterData+0x1C, (char*)ss2Save+(0x188), 0x20);
}

void writeSS2Character(void) {
	// Playable character
	tonccpy((char*)ss2Save+(0x102), &ss2CharacterData, 0x3);
	tonccpy((char*)ss2Save+(0x106), (char*)&ss2CharacterData+0x3, 0x1);
	tonccpy((char*)ss2Save+(0x130), (char*)&ss2CharacterData+0x4, 0x18);
	tonccpy((char*)ss2Save+(0x188), (char*)&ss2CharacterData+0x1C, 0x20);
	tonccpy((char*)ss2Save+(0x1C8), (char*)&ss2CharacterData+0x1C, 0x20);
	tonccpy((char*)ss2Save+(0x208), (char*)&ss2CharacterData+0x1C, 0x20);
	tonccpy((char*)ss2Save+(0x248), (char*)&ss2CharacterData+0x1C, 0x20);
}

void writeSS2CharacterToSave(void) {
	FILE* saveData = fopen(ss2SavePath, "rb+");
	fseek(saveData, 0x102, SEEK_SET);
	fwrite(ss2Save+(0x102), 0x186, 1, saveData);
	fclose(saveData);

	ss2SaveModified = true;
}

void readSS2CharacterFile(const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	// Playable character
	fseek(characterData, 0, SEEK_SET);
	fread((char*)ss2Save+(0x102), 0x3, 1, characterData);
	fseek(characterData, 3, SEEK_SET);
	fread((char*)ss2Save+(0x106), 0x1, 1, characterData);
	fread((char*)ss2Save+(0x130), 0x18, 1, characterData);
	fread((char*)ss2Save+(0x188), 0x20, 1, characterData);
	tonccpy((char*)ss2Save+(0x1C8), (char*)ss2Save+(0x188), 0x20);
	tonccpy((char*)ss2Save+(0x208), (char*)ss2Save+(0x188), 0x20);
	tonccpy((char*)ss2Save+(0x248), (char*)ss2Save+(0x188), 0x20);

	fclose(characterData);
}

void writeSS2CharacterFile(const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	// Playable character
	fwrite((char*)ss2Save+(0x102), 0x3, 1, characterData);
	fwrite((char*)ss2Save+(0x106), 0x1, 1, characterData);
	fwrite((char*)ss2Save+(0x130), 0x18, 1, characterData);
	fwrite((char*)ss2Save+(0x188), 0x20, 1, characterData);

	fclose(characterData);
}

bool getSS2CharacterGender(void) {
	if ((strcmp(ss2PlayerName, "Robz") == 0) && (ss2CharacterData.hairStyle == 0x07)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss2Save[0x102] == 2);
}


/*
	Style Savvy: Fashion Forward
*/
void readSS3Save(void) {
	if (ss3SaveRead) return;

	u32 size = (saveRegion[2]==CFG_REGION_JPN ? 0x120000 : 0x174000);

	FILE* saveData = fopen(ss3SavePath, "rb");
	fread(ss3Save, size, 1, saveData);
	fclose(saveData);

	// Get playable character's name
	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x537DC : 0x54980);
	for (int i = 0; i < 9; i++) {
		ss3PlayerName[i] = ss3Save[offset+(i*2)];
	}

	ss3SaveRead = true;
}

void writeSS3Save(void) {
	u32 size = (saveRegion[2]==CFG_REGION_JPN ? 0x120000 : 0x174000);

	FILE* saveData = fopen(ss3SavePath, "wb");
	fwrite(ss3Save, size, 1, saveData);
	fclose(saveData);

	ss3SaveModified = true;
}

static u32 getSS3CharacterOffset(u16 id) {
	if (id == 0x7D1) {
		// Playable character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x5387E : 0x54A22);
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x830D2 : 0x8427E) + (0x110*id);
	} else if (id == 0x0B87) {
		// Unused/Hidden character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82FC2 : 0x8416E);
	} else if (id == 0x0B55) {
		// Unused/Hidden character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82EB2 : 0x8405E);
	} else if (id == 0x0A8D) {
		// Unused/Hidden character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82DA2 : 0x83F4E);
	} else if (id == 0x0A29 || id == 0x0A2A) {
		// Unused/Hidden character
		id -= 0x0A29;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82B82 : 0x83D2E) + (0x110*id);
	} else if (id >= 0x09C5 && id <= 0x09C9) {
		// Unused/Hidden character
		id -= 0x09C5;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82632 : 0x837DE) + (0x110*id);
	} else if (id == 0x0961) {
		// Unused/Hidden character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x82522 : 0x836CE);
	} else if (id >= 0x08FD && id <= 0x0901) {
		// Unused/Hidden character
		id -= 0x08FD;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x81FD2 : 0x8317E) + (0x110*id);
	} else if (id == 0x0899) {
		// Unused/Hidden character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x81EC2 : 0x8360E);
	} else if (id >= 0x0835 && id <= 0x0852) {
		// Unused/Hidden character
		id -= 0x0835;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x7FEE2 : 0x8101E) + (0x110*id);
	} else if (id >= 0x044C && id <= 0x0469) {
		// Non-playable character
		id -= 0x044C;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x7D9B2 : 0x7EB5E) + (0x110*id);
	} else if (id >= 0x041A && id <= 0x0437) {
		// Non-playable character
		id -= 0x041A;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x7BD92 : 0x7CB7E) + (0x110*id);
	} else if (id >= 0x03E8 && id <= 0x0405) {
		// Non-playable character
		id -= 0x03E8;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x799F2 : 0x7AB9E) + (0x110*id);
	} else if (id >= 0x0352 && id <= 0x036F) {
		// Non-playable character
		id -= 0x0352;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x77A12 : 0x78BBE) + (0x110*id);
	} else if (id >= 0x0320 && id <= 0x033D) {
		// Non-playable character
		id -= 0x0320;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x75A32 : 0x76BDE) + (0x110*id);
	} else if (id >= 0x02EE && id <= 0x030B) {
		// Non-playable character
		id -= 0x02EE;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x73A52 : 0x74BFE) + (0x110*id);
	} else if (id >= 0x02BC && id <= 0x02D9) {
		// Non-playable character
		id -= 0x02BC;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x71962 : 0x72C1E) + (0x110*id);
	} else if (id >= 0x028A && id <= 0x02A7) {
		// Non-playable character
		id -= 0x028A;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x6FA92 : 0x70C3E) + (0x110*id);
	} else if (id >= 0x0258 && id <= 0x0275) {
		// Non-playable character
		id -= 0x0258;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x6DAB2 : 0x6EC5E) + (0x110*id);
	} else if (id >= 0x0226 && id <= 0x0243) {
		// Non-playable character
		id -= 0x0226;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x6BAD2 : 0x6CC7E) + (0x110*id);
	} else if (id >= 0x01F4 && id <= 0x0211) {
		// Non-playable character
		id -= 0x01F4;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x69AF2 : 0x6AC9E) + (0x110*id);
	} else if (id >= 0x01C2 && id <= 0x01DF) {
		// Non-playable character
		id -= 0x01C2;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x67B12 : 0x68CBE) + (0x110*id);
	} else if (id >= 0x0190 && id <= 0x01AD) {
		// Non-playable character
		id -= 0x0190;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x65B32 : 0x66CDE) + (0x110*id);
	} else if (id >= 0x015E && id <= 0x017B) {
		// Non-playable character
		id -= 0x015E;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x63B52 : 0x64CFE) + (0x110*id);
	} else if (id >= 0x012C && id <= 0x0149) {
		// Non-playable character
		id -= 0x012C;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x61B72 : 0x62D1E) + (0x110*id);
	} else if (id >= 0xFA && id <= 0x0117) {
		// Non-playable character
		id -= 0xFA;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x5FB92 : 0x60D3E) + (0x110*id);
	} else if (id >= 0xC8 && id <= 0xE5) {
		// Non-playable character
		id -= 0xC8;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x5DBB2 : 0x5ED5E) + (0x110*id);
	} else if (id >= 0x96 && id <= 0xB3) {
		// Non-playable character
		id -= 0x96;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x5BBD2 : 0x5CD7E) + (0x110*id);
	} else if (id >= 0x64 && id <= 0x81) {
		// Non-playable character
		id -= 0x64;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x59BF2 : 0x5AD9E) + (0x110*id);
	} else if (id >= 0x32 && id <= 0x5E) {
		// Non-playable character
		id -= 0x32;
		return (saveRegion[2]==CFG_REGION_JPN ? 0x56C22 : 0x57DCE) + (0x110*id);
	} else {
		id--;
		// Non-playable character
		return (saveRegion[2]==CFG_REGION_JPN ? 0x54D52 : 0x55EFE) + (0x110*id);
	}
}

void readSS3Character(u16 id) {
	tonccpy(&ss4CharacterData, (char*)ss3Save+getSS3CharacterOffset(id), 0x36);
}

void writeSS3Character(u16 id) {
	tonccpy((char*)ss3Save+getSS3CharacterOffset(id), &ss4CharacterData, 0x36);
}

void toggleSS3Character(u16 id, bool enable) {
	if (id == 0 || id == 0x7D1) return;

	*(u16*)((char*)ss3Save+getSS3CharacterOffset(id)+0xB0) = (enable ? 0x4013 : 0x0000);
}

bool existsSS3Character(u16 id) {
	if (id == 0 || id == 0x7D1) return true;

	u8 byte = *(u8*)((char*)ss3Save+getSS3CharacterOffset(id)+0xB1);
	return (byte & 0xF0) == 0x40;
}

void removeSS3CharacterBag(u16 id) {
	*(u16*)((char*)ss3Save+getSS3CharacterOffset(id)+0x34) = 0;
}

void readSS3CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	fread((char*)ss3Save+getSS3CharacterOffset(id), 0x36, 1, characterData);
	fclose(characterData);
}

void writeSS3CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	fwrite((char*)ss3Save+getSS3CharacterOffset(id), 0x36, 1, characterData);
	fclose(characterData);
}

void writeSS3CharacterToSave(u16 id) {
	FILE* saveData = fopen(ss3SavePath, "rb+");
	fseek(saveData, getSS3CharacterOffset(id), SEEK_SET);
	fwrite(ss3Save+getSS3CharacterOffset(id), 0x804, 1, saveData);
	fclose(saveData);

	ss3SaveModified = true;
}

void backupSS3DLCharacters(const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	// Backup downloaded characters
	fwrite((char*)ss3Save+(getSS3CharacterOffset(0x0BB9)-2), 0x103D4, 1, characterData);
	fclose(characterData);
}

void restoreSS3DLCharacters(const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	// Restore downloaded characters
	fread((char*)ss3Save+(getSS3CharacterOffset(0x0BB9)-2), 0x103D4, 1, characterData);
	fclose(characterData);
}

bool getSS3CharacterGender(u16 id) {
	ss3to4character ss3CharacterGenderCheck;
	tonccpy(&ss3CharacterGenderCheck, (char*)ss3Save+getSS3CharacterOffset(id), 0x36);

	if ((id == 0x7D1) && (strcmp(ss3PlayerName, "Robz") == 0) && (ss3CharacterGenderCheck.hairStyle == 0x27)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss3CharacterGenderCheck.gender == 2);
}

const char* readSS3ProfileName(u16 id) {
	u32 dlCharOffset = (saveRegion[2]==CFG_REGION_JPN ? 0x86F80 : 0x88C5C);

	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		for (int i = 0; i < 16; i++) {
			dlCharName[i] = ss3Save[(dlCharOffset + (0x2B4*id)) + i*2];
		}
		return dlCharName;
	}
	return "???";
}

void readSS3ProfileFile(u16 id, const char* filename) {
	FILE* profileData = fopen(filename, "rb");
	if (!profileData) return;

	u32 dlCharOffset = (saveRegion[2]==CFG_REGION_JPN ? 0x86F80 : 0x88C5C);

	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fread((char*)ss3Save+(dlCharOffset + (0x2B4*id)), 0x224, 1, profileData);
	}
	fclose(profileData);
}

void writeSS3ProfileFile(u16 id, const char* filename) {
	FILE* profileData = fopen(filename, "wb");
	if (!profileData) return;

	u32 dlCharOffset = (saveRegion[2]==CFG_REGION_JPN ? 0x86F80 : 0x88C5C);

	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fwrite((char*)ss3Save+(dlCharOffset + (0x2B4*id)), 0x224, 1, profileData);
	}
	fclose(profileData);
}

void readSS3Emblem(void) {
	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x29A14 : 0x2ABB8);
	tonccpy(&emblemData, (char*)ss3Save+(offset), 0x804);
}

void writeSS3Emblem(void) {
	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x29A14 : 0x2ABB8);
	tonccpy((char*)ss3Save+(offset), &emblemData, 0x804);
}

void writeSS3EmblemToSave(void) {
	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x29A14 : 0x2ABB8);

	FILE* saveData = fopen(ss3SavePath, "rb+");
	fseek(saveData, offset, SEEK_SET);
	fwrite(ss3Save, 0x804, 1, saveData);
	fclose(saveData);

	ss3SaveModified = true;
}

void readSS3EmblemFile(const char* filename) {
	FILE* emblemData = fopen(filename, "rb");
	if (!emblemData) return;

	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x29A14 : 0x2ABB8);
	fread((char*)ss3Save+(offset), 0x804, 1, emblemData);
	fclose(emblemData);
}

void writeSS3EmblemFile(const char* filename) {
	FILE* emblemData = fopen(filename, "wb");
	if (!emblemData) return;

	u32 offset = (saveRegion[2]==CFG_REGION_JPN ? 0x29A14 : 0x2ABB8);
	fwrite((char*)ss3Save+(offset), 0x804, 1, emblemData);
	fclose(emblemData);
}

void readTempEmblemFile(const char* filename) {
	FILE* emblemDataFile = fopen(filename, "rb");
	if (!emblemDataFile) return;

	fread(&emblemData, 0x804, 1, emblemDataFile);
	fclose(emblemDataFile);
}


/*
	Style Savvy: Styling Star
*/
bool foundSS4Save(void) {
	Result res = FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_READ, 0);
	if (R_SUCCEEDED(res)) {
		FSFILE_Close(handle4);
		return true;
	}

	return false;
}

void readSS4Save(void) {
	if (ss4SaveRead) return;

	/*FILE* saveData = fopen(ss4SavePath, "rb");
	fread(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);*/

	u32 bytesRead = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_READ, 0);
	FSFILE_Read(handle4, &bytesRead, 0, ss4Save, (u32)sizeof(ss4Save));
	FSFILE_Close(handle4);

	// Get playable character's name
	for (int i = 0; i < 9; i++) {
		ss4PlayerName[i] = ss4Save[0x24370+(i*2)];
	}

	ss4SaveRead = true;
}

void writeSS4Save(void) {
	/*FILE* saveData = fopen(ss4SavePath, "wb");
	fwrite(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);*/

	u32 bytesWritten = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_WRITE, FS_WRITE_FLUSH);
	FSFILE_Write(handle4, &bytesWritten, 0, ss4Save, (u32)sizeof(ss4Save), 0);
	FSFILE_Close(handle4);

	//ss4SaveModified = true;
}

static u32 getSS4CharacterOffset(u16 id) {
	if (id == 0xBAE) {
		// Playable character
		return 0x2440A;
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		return 0x81CEE + (0x1F8*id);
	} else if (id == 0x0B87) {
		// Unused/Hidden character
		return 0x8111E;
	} else if (id == 0x0B55) {
		// Unused/Hidden character
		return 0x80F26;
	} else if (id >= 0x0AF0 && id <= 0x0AF4) {
		// Unused/Hidden character
		id -= 0x0AF0;
		return 0x8054E + (0x1F8*id);
	} else if (id == 0x0A8D) {
		// Unused/Hidden character
		return 0x80356;
	} else if (id == 0x0A29 || id == 0x0A2A) {
		// Unused/Hidden character
		id -= 0x0A29;
		return 0x7FF66 + (0x1F8*id);
	} else if (id >= 0x09C5 && id <= 0x09C9) {
		// Unused/Hidden character
		id -= 0x09C5;
		return 0x7F58E + (0x1F8*id);
	} else if (id == 0x0961) {
		// Unused/Hidden character
		return 0x7F396;
	} else if (id >= 0x08FD && id <= 0x0901) {
		// Unused/Hidden character
		id -= 0x08FD;
		return 0x7E9BE + (0x1F8*id);
	} else if (id == 0x0899) {
		// Unused/Hidden character
		return 0x7E7C6;
	} else if (id >= 0x0835 && id <= 0x0852) {
		// Unused/Hidden character
		id -= 0x0835;
		return 0x7ACB6 + (0x1F8*id);
	} else if (id == 0x05DC || id == 0x05DD) {
		// Non-playable character
		id -= 0x05DC;
		return 0x7A8C6 + (0x1F8*id);
	} else if (id >= 0x044C && id <= 0x0469) {
		// Non-playable character
		id -= 0x044C;
		return 0x76DB6 + (0x1F8*id);
	} else if (id >= 0x041A && id <= 0x0437) {
		// Non-playable character
		id -= 0x041A;
		return 0x732A6 + (0x1F8*id);
	} else if (id >= 0x03E8 && id <= 0x0408) {
		// Non-playable character
		id -= 0x03E8;
		return 0x6F1AE + (0x1F8*id);
	} else if (id >= 0x0384 && id <= 0x038C) {
		// Non-playable character
		id -= 0x0384;
		return 0x6DFF6 + (0x1F8*id);
	} else if (id >= 0x0352 && id <= 0x036F) {
		// Non-playable character
		id -= 0x0352;
		return 0x6A4E6 + (0x1F8*id);
	} else if (id >= 0x0320 && id <= 0x033D) {
		// Non-playable character
		id -= 0x0320;
		return 0x669D6 + (0x1F8*id);
	} else if (id >= 0x02EE && id <= 0x030B) {
		// Non-playable character
		id -= 0x02EE;
		return 0x62EC6 + (0x1F8*id);
	} else if (id >= 0x02BC && id <= 0x02D9) {
		// Unused/Hidden character
		id -= 0x02BC;
		return 0x5F3B6 + (0x1F8*id);
	} else if (id >= 0x028A && id <= 0x02A7) {
		// Non-playable character
		id -= 0x028A;
		return 0x5B8A6 + (0x1F8*id);
	} else if (id >= 0x0258 && id <= 0x0275) {
		// Non-playable character
		id -= 0x0258;
		return 0x57D96 + (0x1F8*id);
	} else if (id >= 0x0226 && id <= 0x0243) {
		// Non-playable character
		id -= 0x0226;
		return 0x54286 + (0x1F8*id);
	} else if (id >= 0x01F4 && id <= 0x0211) {
		// Non-playable character
		id -= 0x01F4;
		return 0x50776 + (0x1F8*id);
	} else if (id >= 0x01C2 && id <= 0x01DF) {
		// Non-playable character
		id -= 0x01C2;
		return 0x4CC66 + (0x1F8*id);
	} else if (id >= 0x0190 && id <= 0x01AD) {
		// Non-playable character
		id -= 0x0190;
		return 0x49156 + (0x1F8*id);
	} else if (id >= 0x015E && id <= 0x017B) {
		// Non-playable character
		id -= 0x015E;
		return 0x45646 + (0x1F8*id);
	} else if (id >= 0x012C && id <= 0x0149) {
		// Non-playable character
		id -= 0x012C;
		return 0x41B36 + (0x1F8*id);
	} else if (id >= 0xFA && id <= 0x0117) {
		// Non-playable character
		id -= 0xFA;
		return 0x3E026 + (0x1F8*id);
	} else if (id >= 0xC8 && id <= 0xE5) {
		// Non-playable character
		id -= 0xC8;
		return 0x3A516 + (0x1F8*id);
	} else if (id >= 0x96 && id <= 0xB3) {
		// Non-playable character
		id -= 0x96;
		return 0x36A06 + (0x1F8*id);
	} else if (id >= 0x63 && id <= 0x81) {
		// Non-playable character
		id -= 0x63;
		return 0x32CFE + (0x1F8*id);
	} else {
		id--;
		// Non-playable character
		return 0x273EE + (0x1F8*id);
	}
}

static u32 getSS4MewtubeCharacterOffset(int video, int slot) {
	     if (slot < 0) slot = 0;
	else if (slot > 5) slot = 5;

	switch (video) {
		case 0: // Open Plaza: Flying Shiny Day
		default:
			return 0x9BCDA + 0x40*slot;
		case 1: // Open Plaza: Ready Go!!
			return 0x9BE6A + 0x40*slot;
		case 2: // Outdoor Festival: Flying Shiny Day
			return 0x9BFFA + 0x40*slot;
		case 3: // Outdoor Festival: Ready Go!!
			return 0x9C18A + 0x40*slot;
		case 4: // Concert Hall: Higher Higher
			return 0x9C31A + 0x40*slot;
		case 5: // Concert Hall: To the Future
			return 0x9C4AA + 0x40*slot;
		case 6: // Charity Stage: Higher Higher
			return 0x9C63A + 0x40*slot;
		case 7: // Charity Stage: To the Future
			return 0x9C7CA + 0x40*slot;
		case 8: // Live Music Club: Wings
			return 0x9C95A + 0x40*slot;
		case 9: // Live Music Club: Beautiful
			return 0x9CAEA + 0x40*slot;
		case 10: // Celestial Hotel: Make Me Glow
			return 0x9CE0A + 0x40*slot;
		case 11: // Celestial Hotel: Glowing Moon
			return 0x9CF9A + 0x40*slot;
		case 12: // Alinatron On-Air: Fight For Your Style
			return 0x9D12A + 0x40*slot;
		case 13: // Promo Video: Twinkle Fantasia
			return 0x9D2BA + 0x40*slot;
		case 14: // Cinema: Maria
			return 0x9D44A + 0x40*slot;
		case 15: // Concert Hall: Dosukoi Koi Koi (Break It Down)
			return 0x9D5DA + 0x40*slot;
		case 16: // Concert Hall: Radiance
			return 0x9D76A + 0x40*slot;
		case 17: // Open Plaza: Ring a Ding
			return 0x9D8FA + 0x40*slot;
		case 18: // Open Plaza: Bravo
			return 0x9DA8A + 0x40*slot;
	}
}

#define SS3ToSS4EyeAmount 4

// Left: SS3, Right: SS4
static u8 SS3ToSS4EyeTable[SS3ToSS4EyeAmount][2] = { // Offset: 0x05
	{0x04, 0x01},
	{0x06, 0x0A},
	{0x08, 0x17},
	{0x0B, 0x0F},
};

#define SS3ToSS4MouthShapeAmount 6

// Left: SS3, Right: SS4
static u8 SS3ToSS4MouthShapeTable[SS3ToSS4MouthShapeAmount][2] = { // Offset: 0x06
	{0x02, 0x0F},
	{0x03, 0x0B},
	{0x04, 0x0F},
	{0x0C, 0x0B},
	{0x0E, 0x05},
	{0x0D, 0x01},
};

#define SS3ToSS4EyebrowAmount 1

// Left: SS3, Right: SS4
static u8 SS3ToSS4EyebrowTable[SS3ToSS4EyebrowAmount][2] = { // Offset: 0x07
	{0x04, 0x10},
};

#define SS3ToSS4LipstickAmount 18

// Left: SS3, Right: SS4
static u8 SS3ToSS4LipstickTable[SS3ToSS4LipstickAmount][2] = { // Offset: 0x0A
	{0x0F, 0x2B},
	{0x15, 0x5E},
	{0x17, 0x29},
	{0x1A, 0x60},
	{0x1F, 0x08},
	{0x20, 0x39},
	{0x21, 0x12},
	{0x27, 0x2D},
	{0x28, 0x5B},
	{0x39, 0x6A},
	{0x49, 0x71},
	{0x51, 0x0F},
	{0x55, 0x1E},
	{0x5A, 0x63},
	{0x67, 0x02},
	{0x68, 0x07},
	{0x6F, 0x37},
	{0x71, 0x31},
};

#define SS3ToSS4MascaraAmount 11

// Left: SS3, Right: SS4
static u8 SS3ToSS4MascaraTable[SS3ToSS4MascaraAmount][2] = { // Offset: 0x0B
	{0x0B, 0x10},
	{0x0F, 0x10},
	{0x16, 0x20},
	{0x22, 0x2F},
	{0x28, 0x12},
	{0x2A, 0x0E},
	{0x42, 0x10},
	{0x5A, 0x2F},
	{0x5D, 0x2F},
	{0x5F, 0x2F},
	{0x75, 0x10},
};

#define SS3ToSS4EyeshadowAmount 23

// Left: SS3, Right: SS4
static u8 SS3ToSS4EyeshadowTable[SS3ToSS4EyeshadowAmount][2] = { // Offset: 0x0C
	{0x02, 0x3D},
	{0x04, 0x53},
	{0x0A, 0x18},
	{0x13, 0x40},
	{0x14, 0x13},
	{0x15, 0x3B},
	{0x1A, 0x4E},
	{0x1B, 0x59},
	{0x1C, 0x4B},
	{0x21, 0x3E},
	{0x23, 0x2B},
	{0x2A, 0x2B},
	{0x31, 0x2F},
	{0x34, 0x50},
	{0x36, 0x43},
	{0x49, 0x18},
	{0x5C, 0x3F},
	{0x60, 0x5A},
	{0x61, 0x32},
	{0x69, 0x32},
	{0x6A, 0x39},
	{0x6E, 0x06},
	{0x74, 0x29},
};

#define SS3ToSS4EyelinerAmount 14

// Left: SS3, Right: SS4
static u8 SS3ToSS4EyelinerTable[SS3ToSS4EyelinerAmount][2] = { // Offset: 0x0D
	{0x07, 0x30},
	{0x0C, 0x07},
	{0x0F, 0x14},
	{0x10, 0x49},
	{0x17, 0x44},
	{0x1B, 0x5C},
	{0x21, 0x44},
	{0x28, 0x14},
	{0x36, 0x50},
	{0x3B, 0x53},
	{0x45, 0x19},
	{0x4E, 0x2E},
	{0x59, 0x42},
	{0x74, 0x06},
};

#define SS3ToSS4ContactAmount 22

// Left: SS3, Right: SS4
static u8 SS3ToSS4ContactTable[SS3ToSS4ContactAmount][2] = { // Offset: 0x0E
	{0x0A, 0x2F},
	{0x13, 0x17},
	{0x14, 0x52},
	{0x1C, 0x18},
	{0x1E, 0x3B},
	{0x20, 0x10},
	{0x22, 0x3B},
	{0x23, 0x0F},
	{0x24, 0x1C},
	{0x2C, 0x12},
	{0x2D, 0x21},
	{0x2E, 0x30},
	{0x46, 0x1D},
	{0x55, 0x3F},
	{0x5A, 0x30},
	{0x5B, 0x31},
	{0x5D, 0x51},
	{0x64, 0x39},
	{0x6A, 0x18},
	{0x6F, 0x51},
	{0x72, 0x52},
	{0x73, 0x3A},
};

#define SS3ToSS4BlusherAmount 14

// Left: SS3, Right: SS4
static u8 SS3ToSS4BlusherTable[SS3ToSS4BlusherAmount][2] = { // Offset: 0x0F
	{0x01, 0x0F},
	{0x0A, 0x0E},
	{0x15, 0x0E},
	{0x17, 0x11},
	{0x20, 0x0A},
	{0x21, 0x0A},
	{0x23, 0x0E},
	{0x30, 0x04},
	{0x56, 0x0E},
	{0x64, 0x12},
	{0x66, 0x1A},
	{0x67, 0x10},
	{0x68, 0x06},
	{0x69, 0x0C},
};

#define SS3ToSS4HairStyleAmount 22

// Left: SS3, Right: SS4
static u8 SS3ToSS4HairStyleTable[SS3ToSS4HairStyleAmount][2] = { // Offset: 0x15
	{0x01, 0x4B},
	{0x03, 0x4B},
	{0x06, 0x4B},
	{0x07, 0x4C},
	{0x08, 0x4D},
	{0x0A, 0x4E},
	{0x0B, 0x4E},
	{0x0D, 0x4F},
	{0x0F, 0x10},
	{0x12, 0x60},
	{0x13, 0x3C},
	{0x15, 0x63},
	{0x1C, 0x60},
	{0x1F, 0x62},
	{0x25, 0x50},
	{0x28, 0x4E},
	{0x29, 0x51},
	{0x2D, 0x33},
	{0x38, 0x64},
	{0x41, 0x3C},
	{0x48, 0x4E},
	{0x4A, 0x52},
};

void readSS4Character(u16 id) {
	tonccpy(&ss4CharacterData, (char*)ss4Save+getSS4CharacterOffset(id), 0x3E);
}

void readSS3CharacterToSS4(u16 id) {
	toncset(&ss4CharacterData, 0, sizeof(ss3to4character));
	readSS3Character(id);

	// Convert the character
	int i = 0;
	// Eyes
	for (i = 0; i < SS3ToSS4EyeAmount; i++) {
		if (ss4CharacterData.eyes == SS3ToSS4EyeTable[i][0]) {
			ss4CharacterData.eyes = SS3ToSS4EyeTable[i][1];
			break;
		}
	}
	// Mouth shape
	for (i = 0; i < SS3ToSS4MouthShapeAmount; i++) {
		if (ss4CharacterData.mouthShape == SS3ToSS4MouthShapeTable[i][0]) {
			ss4CharacterData.mouthShape = SS3ToSS4MouthShapeTable[i][1];
			break;
		}
	}
	// Eyebrows
	for (i = 0; i < SS3ToSS4EyebrowAmount; i++) {
		if (ss4CharacterData.eyebrows == SS3ToSS4EyebrowTable[i][0]) {
			ss4CharacterData.eyebrows = SS3ToSS4EyebrowTable[i][1];
			break;
		}
	}
	// Lipstick
	for (i = 0; i < SS3ToSS4LipstickAmount; i++) {
		if (ss4CharacterData.lipstickColor == SS3ToSS4LipstickTable[i][0]) {
			ss4CharacterData.lipstickColor = SS3ToSS4LipstickTable[i][1];
			break;
		}
	}
	// Mascara
	for (i = 0; i < SS3ToSS4MascaraAmount; i++) {
		if (ss4CharacterData.mascaraColor == SS3ToSS4MascaraTable[i][0]) {
			ss4CharacterData.mascaraColor = SS3ToSS4MascaraTable[i][1];
			break;
		}
	}
	// Eyeshadow
	for (i = 0; i < SS3ToSS4EyeshadowAmount; i++) {
		if (ss4CharacterData.eyeshadowColor == SS3ToSS4EyeshadowTable[i][0]) {
			ss4CharacterData.eyeshadowColor = SS3ToSS4EyeshadowTable[i][1];
			break;
		}
	}
	// Eyeliner
	for (i = 0; i < SS3ToSS4EyelinerAmount; i++) {
		if (ss4CharacterData.eyelinerColor == SS3ToSS4EyelinerTable[i][0]) {
			ss4CharacterData.eyelinerColor = SS3ToSS4EyelinerTable[i][1];
			break;
		}
	}
	// Contacts
	for (i = 0; i < SS3ToSS4ContactAmount; i++) {
		if (ss4CharacterData.contactColor == SS3ToSS4ContactTable[i][0]) {
			ss4CharacterData.contactColor = SS3ToSS4ContactTable[i][1];
			break;
		}
	}
	// Blusher
	for (i = 0; i < SS3ToSS4BlusherAmount; i++) {
		if (ss4CharacterData.blusherColor == SS3ToSS4BlusherTable[i][0]) {
			ss4CharacterData.blusherColor = SS3ToSS4BlusherTable[i][1];
			break;
		}
	}
	// Overlaying Eyebrows
	for (i = 0; i < SS3ToSS4EyebrowAmount; i++) {
		if (ss4CharacterData.eyebrowOverlay == SS3ToSS4EyebrowTable[i][0]) {
			ss4CharacterData.eyebrowOverlay = SS3ToSS4EyebrowTable[i][1];
			break;
		}
	}
	// Hair style
	for (i = 0; i < SS3ToSS4HairStyleAmount; i++) {
		if (ss4CharacterData.hairStyle == SS3ToSS4HairStyleTable[i][0]) {
			ss4CharacterData.hairStyle = SS3ToSS4HairStyleTable[i][1];
			break;
		}
	}
}

void writeSS4Character(u16 id) {
	tonccpy((char*)ss4Save+getSS4CharacterOffset(id), &ss4CharacterData, 0x3E);
}

void writeSS4CharacterToSave(u16 id) {
	u32 bytesWritten = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_WRITE, FS_WRITE_FLUSH);
	FSFILE_Write(handle4, &bytesWritten, getSS4CharacterOffset(id), ss4Save+getSS4CharacterOffset(id), 0x3E, 0);
	FSFILE_Close(handle4);
}

u16 getSS4AssistantCharacterId(void) {
	return *(u16*)((char*)ss4Save+0x8BF96);
}

void writeSS4AssistantCharacterId(u16 id) {
	*(u16*)((char*)ss4Save+0x8BF96) = id;
}

void writeSS4AssistantCharacterToSave(void) {
	u32 bytesWritten = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_WRITE, FS_WRITE_FLUSH);
	FSFILE_Write(handle4, &bytesWritten, 0x8BF96, ss4Save+0x8BF96, 2, 0);
	FSFILE_Close(handle4);
}

void writeSS4MewtubeCharacter(int video, int slot) {
	tonccpy((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot), &ss4CharacterData, 0x3E);
}

void writeSS4MewtubeCharacterToSave(int video, int slot) {
	u32 bytesWritten = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_WRITE, FS_WRITE_FLUSH);
	FSFILE_Write(handle4, &bytesWritten, getSS4MewtubeCharacterOffset(video, slot)-2, ss4Save+getSS4MewtubeCharacterOffset(video, slot)-2, 0x40, 0);
	FSFILE_Close(handle4);
}

void revertSS4MewtubeCharacter(int video, int slot) {
	if (slot < 3) slot += 3;

	*(u16*)((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot)-2) = *(u16*)((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot-3)-2);
	tonccpy((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot), (char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot-3), 0x3E);
}

bool existsSS4Character(u16 id) {
	if (id == 0 || id == 0xBAE) return true;

	u8 byte = *(u8*)((char*)ss4Save+getSS4CharacterOffset(id)+0x181);
	return (byte & 0xF0) == 0x40;
}

void readSS4CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	fread((char*)ss4Save+getSS4CharacterOffset(id), 0x3E, 1, characterData);
	fclose(characterData);
}

void writeSS4CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	fwrite((char*)ss4Save+getSS4CharacterOffset(id), 0x3E, 1, characterData);
	fclose(characterData);
}

u16 getSS4MewtubeCharacterId(int video, int slot) {
	return *(u16*)((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot)-2);
}

void writeSS4MewtubeCharacterId(u16 id, int video, int slot) {
	*(u16*)((char*)ss4Save+getSS4MewtubeCharacterOffset(video, slot)-2) = id;
}

bool getSS4CharacterGender(u16 id) {
	ss3to4character ss4CharacterGenderCheck;
	tonccpy(&ss4CharacterGenderCheck, (char*)ss4Save+getSS4CharacterOffset(id), 0x3E);

	if ((id == 0xBAE) && (strcmp(ss4PlayerName, "Robz") == 0) && (ss4CharacterGenderCheck.hairStyle == 0x6D)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterGenderCheck.gender == 2);
}

const char* readSS4ProfileName(u16 id) {
	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		for (int i = 0; i < 16; i++) {
			dlCharName[i] = ss4Save[(0x895BC + (0x150*id)) + i*2];
		}
		return dlCharName;
	}
	return "???";
}

void readSS4Emblem(int id) {
	switch (id) {
		case 0:
			tonccpy(&emblemData, (char*)ss4Save+(0x228FC), 0x804);
			break;
		case 1:
			tonccpy(&emblemData, (char*)ss4Save+(0x23100), 0x804);
			break;
		case 2:
			tonccpy(&emblemData, (char*)ss4Save+(0x23904), 0x804);
			break;
	}
}

void writeSS4Emblem(int id) {
	switch (id) {
		case 0:
			tonccpy((char*)ss4Save+(0x228FC), &emblemData, 0x804);
			break;
		case 1:
			tonccpy((char*)ss4Save+(0x23100), &emblemData, 0x804);
			break;
		case 2:
			tonccpy((char*)ss4Save+(0x23904), &emblemData, 0x804);
			break;
	}
}

void writeSS4EmblemToSave(int id) {
	u32 bytesWritten = 0;
	FSUSER_OpenFile(&handle4, archive4, fsMakePath(PATH_UTF16, (const void*)UTF8toUTF16(ss4SavePath).data()), FS_OPEN_WRITE, FS_WRITE_FLUSH);

	switch (id) {
		case 0:
			FSFILE_Write(handle4, &bytesWritten, 0x228FC, ss4Save+(0x228FC), 0x804, 0);
			break;
		case 1:
			FSFILE_Write(handle4, &bytesWritten, 0x23100, ss4Save+(0x23100), 0x804, 0);
			break;
		case 2:
			FSFILE_Write(handle4, &bytesWritten, 0x23904, ss4Save+(0x23904), 0x804, 0);
			break;
	}

	FSFILE_Close(handle4);
}

void readSS4EmblemFile(int id, const char* filename) {
	FILE* emblemData = fopen(filename, "rb");
	if (!emblemData) return;

	switch (id) {
		case 0:
			fread((char*)ss4Save+(0x228FC), 0x804, 1, emblemData);
			break;
		case 1:
			fread((char*)ss4Save+(0x23100), 0x804, 1, emblemData);
			break;
		case 2:
			fread((char*)ss4Save+(0x23904), 0x804, 1, emblemData);
			break;
	}
	fclose(emblemData);
}

void writeSS4EmblemFile(int id, const char* filename) {
	FILE* emblemData = fopen(filename, "wb");
	if (!emblemData) return;

	switch (id) {
		case 0:
			fwrite((char*)ss4Save+(0x228FC), 0x804, 1, emblemData);
			break;
		case 1:
			fwrite((char*)ss4Save+(0x23100), 0x804, 1, emblemData);
			break;
		case 2:
			fwrite((char*)ss4Save+(0x23904), 0x804, 1, emblemData);
			break;
	}
	fclose(emblemData);
}
