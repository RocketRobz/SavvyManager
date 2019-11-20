#include <3ds.h>
#include <stdio.h>
#include <cstring>
#include "savedata.h"
#include "tonccpy.h"

ss3to4character ss4CharacterData;

char ss3Save[0x174000];
char ss4Save[0xF0000];

char ss4PlayerName[10] = {0};

static bool ss4SaveRead = false;

void readSS4Save(void) {
	if (ss4SaveRead) return;

	FILE* saveData = fopen("sdmc:/3ds/Checkpoint/extdata/0x01C25 Style Savvy  Styling Star/SavvyManager/savedata.dat", "rb");
	fread(ss4Save, (int)sizeof(ss4Save), 1, saveData);
	fclose(saveData);

	// Get playable character's name
	for (int i = 0; i < 9; i++) {
		ss4PlayerName[i] = ss4Save[0x24370+(i*2)];
	}

	ss4SaveRead = true;
}

void writeSS4Save(void) {
	FILE* saveData = fopen("sdmc:/3ds/Checkpoint/extdata/0x01C25 Style Savvy  Styling Star/SavvyManager/savedata.dat", "wb");
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

bool getSS4CharacterGender(u16 id) {
	readSS4Character(id);

	if ((id == 0) && (strcmp(ss4PlayerName, "Robz") == 0)) {
		return true;	// Robz is male, so return male
	}

	// true = male, false = female
	return (ss4CharacterData.gender == 2);
}
