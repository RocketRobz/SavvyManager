#include <3ds.h>
#include <stdio.h>
#include <cstring>
#include "savedata.h"
#include "tonccpy.h"

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

static bool ss1SaveRead = false;
static bool ss2SaveRead = false;
static bool ss3SaveRead = false;
static bool ss4SaveRead = false;

static bool ss2SaveModified = false;
static bool ss3SaveModified = false;
static bool ss4SaveModified = false;

void commitSaveData(void) {
	if (ss2SaveModified) {
		archiveCommitSaveData("ss2");
	}
	if (ss3SaveModified) {
		archiveCommitSaveData("ss3");
	}
	if (ss4SaveModified) {
		archiveCommitSaveData("ss4");
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

	FILE* saveData = fopen(ss3SavePath, "rb");
	fread(ss3Save, (int)sizeof(ss3Save), 1, saveData);
	fclose(saveData);

	// Get playable character's name
	for (int i = 0; i < 9; i++) {
		ss3PlayerName[i] = ss3Save[0x54980+(i*2)];
	}

	ss3SaveRead = true;
}

void writeSS3Save(void) {
	FILE* saveData = fopen(ss3SavePath, "wb");
	fwrite(ss3Save, (int)sizeof(ss3Save), 1, saveData);
	fclose(saveData);

	ss3SaveModified = true;
}

void readSS3Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy(&ss4CharacterData, (char*)ss3Save+(0x54A22), 0x36);
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		tonccpy(&ss4CharacterData, (char*)ss3Save+(0x8427E + (0x110*id)), 0x36);
	} else {
		id--;
		// Non-playable character
		tonccpy(&ss4CharacterData, (char*)ss3Save+(0x55EFE + (0x110*id)), 0x36);
	}
}

void writeSS3Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy((char*)ss3Save+(0x54A22), &ss4CharacterData, 0x36);
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		tonccpy((char*)ss3Save+(0x8427E + (0x110*id)), &ss4CharacterData, 0x36);
	} else {
		id--;
		// Non-playable character
		tonccpy((char*)ss3Save+(0x55EFE + (0x110*id)), &ss4CharacterData, 0x36);
	}
}

void toggleSS3Character(u16 id, bool enable) {
	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		*(u16*)(ss3Save+(0x8432E + (0x110*id))) = (enable ? 0x4013 : 0x0000);
	}
}

void removeSS3CharacterBag(u16 id) {
	if (id == 0) {
		// Playable character
		*(u16*)(ss3Save+(0x54A56)) = 0;
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		*(u16*)(ss3Save+(0x842B2 + (0x110*id))) = 0;
	} else {
		id--;
		// Non-playable character
		*(u16*)(ss3Save+(0x55F32 + (0x110*id))) = 0;
	}
}

void readSS3CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	if (id == 0) {
		// Playable character
		fread((char*)ss3Save+(0x54A22), 0x36, 1, characterData);
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fread((char*)ss3Save+(0x8427E + (0x110*id)), 0x36, 1, characterData);
	} else {
		id--;
		// Non-playable character
		fread((char*)ss3Save+(0x55EFE + (0x110*id)), 0x36, 1, characterData);
	}
	fclose(characterData);
}

void writeSS3CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	if (id == 0) {
		// Playable character
		fwrite((char*)ss3Save+(0x54A22), 0x36, 1, characterData);
	} else if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fwrite((char*)ss3Save+(0x8427E + (0x110*id)), 0x36, 1, characterData);
	} else  {
		id--;
		// Non-playable character
		fwrite((char*)ss3Save+(0x55EFE + (0x110*id)), 0x36, 1, characterData);
	}
	fclose(characterData);
}

bool getSS3CharacterGender(u16 id) {
	readSS3Character(id);

	if ((id == 0) && (strcmp(ss3PlayerName, "Robz") == 0) && (ss4CharacterData.hairStyle == 0x27)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterData.gender == 2);
}

void readSS3ProfileFile(u16 id, const char* filename) {
	FILE* profileData = fopen(filename, "rb");
	if (!profileData) return;

	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fread((char*)ss3Save+(0x88C5C + (0x2B4*id)), 0x224, 1, profileData);
	}
	fclose(profileData);
}

void writeSS3ProfileFile(u16 id, const char* filename) {
	FILE* profileData = fopen(filename, "wb");
	if (!profileData) return;

	if (id >= 0x0BB9) {
		// Downloaded character
		id -= 0x0BB9;
		fwrite((char*)ss3Save+(0x88C5C + (0x2B4*id)), 0x224, 1, profileData);
	}
	fclose(profileData);
}

void readSS3Emblem(void) {
	tonccpy(&emblemData, (char*)ss3Save+(0x2ABB8), 0x804);
}

void writeSS3Emblem(void) {
	tonccpy((char*)ss3Save+(0x2ABB8), &emblemData, 0x804);
}

void readSS3EmblemFile(const char* filename) {
	FILE* emblemData = fopen(filename, "rb");
	if (!emblemData) return;

	fread((char*)ss3Save+(0x2ABB8), 0x804, 1, emblemData);
	fclose(emblemData);
}

void writeSS3EmblemFile(const char* filename) {
	FILE* emblemData = fopen(filename, "wb");
	if (!emblemData) return;

	fwrite((char*)ss3Save+(0x2ABB8), 0x804, 1, emblemData);
	fclose(emblemData);
}


/*
	Style Savvy: Styling Star
*/
void readSS4Save(void) {
	if (ss4SaveRead) return;

	FILE* saveData = fopen(ss4SavePath, "rb");
	fread(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);

	// Get playable character's name
	for (int i = 0; i < 9; i++) {
		ss4PlayerName[i] = ss4Save[0x24370+(i*2)];
	}

	ss4SaveRead = true;
}

void writeSS4Save(void) {
	FILE* saveData = fopen(ss4SavePath, "wb");
	fwrite(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);

	ss4SaveModified = true;
}

void readSS4Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy(&ss4CharacterData, (char*)ss4Save+(0x2440A), 0x3E);
	} else {
		id--;
		// Non-playable character
		tonccpy(&ss4CharacterData, (char*)ss4Save+(0x273EE + (0x1F8*id)), 0x3E);
	}
}

void writeSS4Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy((char*)ss4Save+(0x2440A), &ss4CharacterData, 0x3E);
	} else {
		id--;
		// Non-playable character
		tonccpy((char*)ss4Save+(0x273EE + (0x1F8*id)), &ss4CharacterData, 0x3E);
	}
}

void readSS4CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	if (id == 0) {
		// Playable character
		fread((char*)ss4Save+(0x2440A), 0x3E, 1, characterData);
	} else {
		id--;
		// Non-playable character
		fread((char*)ss4Save+(0x273EE + (0x1F8*id)), 0x3E, 1, characterData);
	}
	fclose(characterData);
}

void writeSS4CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "wb");
	if (!characterData) return;

	if (id == 0) {
		// Playable character
		fwrite((char*)ss4Save+(0x2440A), 0x3E, 1, characterData);
	} else {
		id--;
		// Non-playable character
		fwrite((char*)ss4Save+(0x273EE + (0x1F8*id)), 0x3E, 1, characterData);
	}
	fclose(characterData);
}

bool getSS4CharacterGender(u16 id) {
	readSS4Character(id);

	if ((id == 0) && (strcmp(ss4PlayerName, "Robz") == 0) && (ss4CharacterData.hairStyle == 0x6D)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterData.gender == 2);
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
