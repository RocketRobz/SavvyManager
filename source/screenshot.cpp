#include <3ds.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		// access

#include "screenshot.h"

extern void renderTopScreenSubPixels(void);

static Result Screenshot_GenerateScreenshot(const char *path) {
	int x = 0, y = 0;
	int width = gfxIsWide() ? 800 : 400;
	int height = gfxIsWide() ? 480 : 240;
	int fileSize = gfxIsWide() ? 576000*2 : 288000;
	size_t size = 0x36;

	// Get top/bottom framebuffers
	//u8 *top_framebuf = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	u8 *bottom_framebuf = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

	u8 *buf = (u8*)linearAlloc(size + fileSize);
	memset(buf, 0, size + fileSize);
	buf[size + fileSize] = 0;

	*(u16*)&buf[0x0] = 0x4D42;
	*(u32*)&buf[0x2] = size + fileSize;
	*(u32*)&buf[0xA] = size;
	*(u32*)&buf[0xE] = 0x28;
	*(u32*)&buf[0x12] = width;
	*(u32*)&buf[0x16] = height;
	*(u32*)&buf[0x1A] = 0x00180001;
	*(u32*)&buf[0x22] = fileSize;

	// Generate top left
	u8 *framebuf = bottom_framebuf;

	if (gfxIsWide()) {
		// Left half
		int x2 = 0;
		int y2 = 0;
	  for (int i = 0; i <= 1; i++) {
		if (i==1) renderTopScreenSubPixels();
		x2 = 0;
		y2 = i;
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 400; x++) {
				int si = ((239 - y) + (x * 240)) * 3;
				int di = size + (x2 + ((479 - y2) * 800)) * 3;
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
				x2++;
				if (x2 == 400) x2 = 0;
			}
			y2 += 2;
		}
		// Right half
		x2 = 400;
		y2 = i;
		for (y = 0; y < 240; y++) {
			for (x = 400; x < 800; x++) {
				int si = ((239 - y) + (x * 240)) * 3;
				int di = size + (x2 + ((479 - y2) * 800)) * 3;
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
				x2++;
				if (x2 == 800) x2 = 400;
			}
			y2 += 2;
		}
	  }
	} else {
		for (y = 0; y < 240; y++) {
			for (x = 0; x < 400; x++) {
				int si = ((239 - y) + (x * 240)) * 3;
				int di = size + (x + ((239 - y) * 400)) * 3;
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
				buf[di++] = framebuf[si++];
			}
		}
	}

	// Generate bottom right
	/*framebuf = top_framebuf;

	for (y = 0; y < 240; y++) {
		for (x = 0; x < 320; x++) {
			int si = ((239 - y) + (x * 240)) * 3;
			int di = size + ((x+40) + ((239 - y) * 400)) * 3;
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
		}

		// Make adjustments for the smaller width
		for (x = 0; x < 40; x++) {
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}

		for (x = 360; x < 400; x++) {
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}
	}*/

	FILE* bmpFile = fopen(path, "wb");
	fwrite((u32 *)buf, 1, size + fileSize, bmpFile);
	fclose(bmpFile);
	if (access(path, F_OK) != 0) {
		free(buf);
	}

	linearFree(buf);
	return 0;
}

static void Screenshot_GenerateFilename(int count, char *file_name) {
	time_t t = time(0);
	int day = localtime(&t)->tm_mday;
	int month = localtime(&t)->tm_mon + 1;
	int year = localtime(&t)->tm_year + 1900;
	
	mkdir("sdmc:/3ds/SavvyManager/screenshots", 0777);

	sprintf(file_name, "sdmc:/3ds/SavvyManager/screenshots/Screenshot_%02d%02d%02d-%i.bmp", year, month, day, count);
}

void Screenshot_Capture(void) {
	int num = 0;
	static char file_name[256];
	sprintf(file_name, "%s", "screenshot");
	Screenshot_GenerateFilename(num, file_name);

	while (access(file_name, F_OK) == 0) {
		num++;
		Screenshot_GenerateFilename(num, file_name);
	}

	Screenshot_GenerateScreenshot(file_name);
	num++;
}
