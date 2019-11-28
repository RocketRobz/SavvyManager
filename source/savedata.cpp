#include <3ds.h>
#include <stdio.h>
#include <cstring>
#include "savedata.h"
#include "tonccpy.h"

ss3to4character ss4CharacterData;

char ss2Save[0x31736];
char ss3Save[0x174000];
char ss4Save[0xF0000];

char ss2PlayerName[10] = {0};
char ss3PlayerName[10] = {0};
char ss4PlayerName[10] = {0};

static bool ss2SaveRead = false;
static bool ss3SaveRead = false;
static bool ss4SaveRead = false;

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
	remove(ss2SavePath);
	FILE* saveData = fopen(ss2SavePath, "wb");
	fwrite(ss2Save, (int)sizeof(ss2Save), 1, saveData);
	fclose(saveData);
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
	remove(ss3SavePath);
	FILE* saveData = fopen(ss3SavePath, "wb");
	fwrite(ss3Save, (int)sizeof(ss3Save), 1, saveData);
	fclose(saveData);
}

void readSS3Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy(&ss4CharacterData, (char*)ss3Save+(0x54A22), 0x36);
	} else {
		// Non-playable character
		tonccpy(&ss4CharacterData, (char*)ss3Save+(0x55EFE + (0x110*id)), 0x36);
	}
}

void writeSS3Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy((char*)ss3Save+(0x54A22), &ss4CharacterData, 0x36);
	} else {
		// Non-playable character
		tonccpy((char*)ss3Save+(0x55EFE + (0x110*id)), &ss4CharacterData, 0x36);
	}
}

void readSS3CharacterFile(u16 id, const char* filename) {
	FILE* characterData = fopen(filename, "rb");
	if (!characterData) return;

	if (id == 0) {
		// Playable character
		fread((char*)ss3Save+(0x54A22), 0x36, 1, characterData);
	} else {
		// Non-playable character
		fread((char*)ss3Save+(0x55EFE + (0x110*id)), 0x36, 1, characterData);
	}
	fclose(characterData);
}

bool getSS3CharacterGender(u16 id) {
	readSS3Character(id);

	if ((id == 0) && (strcmp(ss3PlayerName, "Robz") == 0)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterData.gender == 2);
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
	remove(ss4SavePath);
	FILE* saveData = fopen(ss4SavePath, "wb");
	fwrite(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);
}

void readSS4Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy(&ss4CharacterData, (char*)ss4Save+(0x2440A), 0x3E);
	} else {
		// Non-playable character
		tonccpy(&ss4CharacterData, (char*)ss4Save+(0x273EE + (0x1F8*id)), 0x3E);
	}
}

void writeSS4Character(u16 id) {
	if (id == 0) {
		// Playable character
		tonccpy((char*)ss4Save+(0x2440A), &ss4CharacterData, 0x3E);
	} else {
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
		// Non-playable character
		fread((char*)ss4Save+(0x273EE + (0x1F8*id)), 0x3E, 1, characterData);
	}
	fclose(characterData);
}

bool getSS4CharacterGender(u16 id) {
	readSS4Character(id);

	if ((id == 0) && (strcmp(ss4PlayerName, "Robz") == 0)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterData.gender == 2);
}
