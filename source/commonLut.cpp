#include <3ds.h>
#include <stdio.h>
#include <unistd.h>		// access
#include "tonccpy.h"

extern u8 saveRegion[4];

char commonLutData[0x88F0];

void loadCommonLut(int num) {
	const char* commonLutPath = "sdmc:/luma/titles/00040000001C2500/romfs/USA/Common/CommonLut.bch";

	switch (saveRegion[3]) {
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			commonLutPath = "sdmc:/luma/titles/00040000001C2600/romfs/EUR/Common/CommonLut.bch";
			break;
		case CFG_REGION_JPN:
			commonLutPath = "sdmc:/luma/titles/000400000019F600/romfs/JPN/Common/CommonLut.bch";
			break;
	}

	const char* commonLutName = "";
	switch (num) {
		default:
			remove(commonLutPath);
			return;
		case 1:
			commonLutName = "No cel-shade";
			break;
		case 2:
			commonLutName = "Fashion Dreamer";
			break;
	}

	char commonLutSrc[80];
	sprintf(commonLutSrc, "romfs:/character/Styling Star/CommonLut/%s.bch", commonLutName);

	FILE* file = fopen(commonLutSrc, "rb");
	fread(commonLutData, 1, (int)sizeof(commonLutData), file);
	fclose(file);

	file = fopen(commonLutPath, "wb");
	fwrite(commonLutData, 1, (int)sizeof(commonLutData), file);
	fclose(file);
}
