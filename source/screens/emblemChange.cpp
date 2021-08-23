#include "emblemChange.hpp"
#include "screenvars.h"
#include "whatToDo.hpp"

#include "savedata.h"
#include "file_browse.h"

#include "import_emblemnames.h"
#include <unistd.h>

EmblemChange::EmblemChange() {
	getMaxEmblems();
}

void EmblemChange::getMaxEmblems() {
	if (subScreenMode == 2) {
		if (importPage == 1) {
			totalEmblems = numberOfExportedEmblems-1;
		} else {
			totalEmblems = 10;
		}
	} else if (highlightedGame == 3) {
		totalEmblems = 2;
		readSS4Save();
		readSS4Emblem(cursorPosition);
	} else {
		totalEmblems = 0;
		readSS3Save();
		readSS3Emblem();
	}

	if (!modeInited) {
		renderEmblem();
		modeInited = true;
	}
}

static u32 emblemPalette[19][16] = 
{
	{ // 1
		C2D_Color32(0, 0, 0, 0),
		C2D_Color32(255, 0, 0, 255),
		C2D_Color32(247, 158, 0, 255),
		C2D_Color32(255, 247, 16, 255),
		C2D_Color32(0, 223, 25, 255),
		C2D_Color32(0, 166, 25, 255),
		C2D_Color32(140, 239, 255, 255),
		C2D_Color32(0, 89, 230, 255),
		C2D_Color32(99, 89, 255, 255),
		C2D_Color32(187, 89, 255, 255),
		C2D_Color32(255, 166, 164, 255),
		C2D_Color32(148, 0, 0, 255),
		C2D_Color32(156, 105, 49, 255),
		C2D_Color32(132, 129, 132, 255),
		C2D_Color32(255, 255, 255, 255),
		C2D_Color32(0, 0, 0, 255)
	},
	{ // 2
		C2D_Color32(0, 0, 0, 0),
		C2D_Color32(230, 0, 16, 255),
		C2D_Color32(255, 24, 41, 255),
		C2D_Color32(255, 65, 0, 255),
		C2D_Color32(255, 166, 123, 255),
		C2D_Color32(247, 206, 0, 255),
		C2D_Color32(255, 203, 0, 255),
		C2D_Color32(255, 255, 0, 255),
		C2D_Color32(255, 158, 164, 255),
		C2D_Color32(255, 113, 181, 255),
		C2D_Color32(255, 140, 40, 255),
		C2D_Color32(148, 0, 0, 255),
		C2D_Color32(255, 81, 214, 255),
		C2D_Color32(189, 57, 156, 255),
		C2D_Color32(255, 255, 255, 255),
		C2D_Color32(0, 0, 0, 255)
	},
	{ // 3
		C2D_Color32(0, 0, 0, 0),
		C2D_Color32(41, 49, 148, 255),
		C2D_Color32(0, 89, 230, 255),
		C2D_Color32(0, 121, 255, 255),
		C2D_Color32(107, 190, 255, 255),
		C2D_Color32(123, 214, 247, 255),
		C2D_Color32(206, 239, 255, 255),
		C2D_Color32(25, 190, 181, 255),
		C2D_Color32(173, 239, 66, 255),
		C2D_Color32(0, 223, 25, 255),
		C2D_Color32(0, 166, 25, 255),
		C2D_Color32(173, 166, 255, 255),
		C2D_Color32(99, 89, 255, 255),
		C2D_Color32(90, 40, 189, 255),
		C2D_Color32(255, 255, 255, 255),
		C2D_Color32(0, 0, 0, 255)
	},
	{ // 4
		C2D_Color32(0, 0, 0, 0),
		C2D_Color32(255, 97, 115, 255),
		C2D_Color32(255, 170, 99, 255),
		C2D_Color32(255, 223, 99, 255),
		C2D_Color32(255, 247, 156, 255),
		C2D_Color32(156, 255, 49, 255),
		C2D_Color32(90, 231, 90, 255),
		C2D_Color32(74, 255, 230, 255),
		C2D_Color32(189, 255, 255, 255),
		C2D_Color32(99, 206, 255, 255),
		C2D_Color32(132, 150, 255, 255),
		C2D_Color32(181, 129, 255, 255),
		C2D_Color32(238, 113, 255, 255),
		C2D_Color32(255, 158, 197, 255),
		C2D_Color32(255, 255, 255, 255),
		C2D_Color32(99, 97, 99, 255)
	},
	{ // 5
		C2D_Color32(0, 0, 0, 0),
		C2D_Color32(206, 121, 164, 255),
		C2D_Color32(255, 150, 156, 255),
		C2D_Color32(255, 158, 164, 255),
		C2D_Color32(255, 198, 197, 255),
		C2D_Color32(255, 129, 189, 255),
		C2D_Color32(255, 170, 122, 255),
		C2D_Color32(255, 206, 122, 255),
		C2D_Color32(255, 223, 156, 255),
		C2D_Color32(255, 190, 123, 255),
		C2D_Color32(189, 214, 247, 255),
		C2D_Color32(197, 214, 132, 255),
		C2D_Color32(206, 190, 255, 255),
		C2D_Color32(181, 158, 255, 255),
		C2D_Color32(255, 255, 255, 255),
		C2D_Color32(99, 97, 99, 255)
	},
	{ // 6
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x5a, 0x8a, 0xce, 255),
		C2D_Color32(0x84, 0xbe, 0xff, 255),
		C2D_Color32(0xbd, 0xd6, 0xf7, 255),
		C2D_Color32(0xb5, 0xef, 0xee, 255),
		C2D_Color32(0x63, 0xce, 0xce, 255),
		C2D_Color32(0x94, 0xef, 0xad, 255),
		C2D_Color32(0xff, 0xf7, 0x7b, 255),
		C2D_Color32(0xd6, 0xef, 0x6b, 255),
		C2D_Color32(0xc5, 0xd6, 0x84, 255),
		C2D_Color32(0xff, 0x9e, 0xa4, 255),
		C2D_Color32(0xff, 0xbe, 0x7b, 255),
		C2D_Color32(0xb5, 0xbe, 0xff, 255),
		C2D_Color32(0xa4, 0x96, 0xff, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x63, 0x61, 0x63, 255)
	},
	{ // 7
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xce, 0x59, 0x8c, 255),
		C2D_Color32(0xff, 0xbe, 0xc5, 255),
		C2D_Color32(0xb5, 0x81, 0xd6, 255),
		C2D_Color32(0xbd, 0xce, 0xf7, 255),
		C2D_Color32(0x5a, 0x8a, 0xce, 255),
		C2D_Color32(0xf7, 0xef, 0xb5, 255),
		C2D_Color32(0xee, 0xd6, 0x9c, 255),
		C2D_Color32(0xe6, 0xbe, 0x52, 255),
		C2D_Color32(0x84, 0x61, 0x42, 255),
		C2D_Color32(0x9c, 0xd6, 0xbd, 255),
		C2D_Color32(0xe6, 0xd6, 0xd6, 255),
		C2D_Color32(0xa4, 0x96, 0x94, 255),
		C2D_Color32(0x52, 0x49, 0x4a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 8
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x63, 0x18, 0x10, 255),
		C2D_Color32(0x84, 0x20, 0x31, 255),
		C2D_Color32(0xce, 0x59, 0x8c, 255),
		C2D_Color32(0xff, 0xbe, 0xc5, 255),
		C2D_Color32(0xb5, 0x81, 0xd6, 255),
		C2D_Color32(0xf7, 0xef, 0xb5, 255),
		C2D_Color32(0xee, 0xd6, 0x9c, 255),
		C2D_Color32(0xe6, 0xbe, 0x52, 255),
		C2D_Color32(0x84, 0x61, 0x42, 255),
		C2D_Color32(0xff, 0x71, 0xb5, 255),
		C2D_Color32(0xe6, 0xd6, 0xd6, 255),
		C2D_Color32(0xa4, 0x96, 0x94, 255),
		C2D_Color32(0x52, 0x49, 0x4a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 9
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x31, 0x69, 0xb5, 255),
		C2D_Color32(0x5a, 0x8a, 0xce, 255),
		C2D_Color32(0xbd, 0xce, 0xf7, 255),
		C2D_Color32(0x9c, 0xd6, 0xbd, 255),
		C2D_Color32(0x00, 0xb2, 0xc5, 255),
		C2D_Color32(0xf7, 0xef, 0xb5, 255),
		C2D_Color32(0xee, 0xd6, 0x9c, 255),
		C2D_Color32(0xe6, 0xbe, 0x52, 255),
		C2D_Color32(0x84, 0x61, 0x42, 255),
		C2D_Color32(0x63, 0x51, 0xa4, 255),
		C2D_Color32(0xe6, 0xd6, 0xd6, 255),
		C2D_Color32(0xa4, 0x96, 0x94, 255),
		C2D_Color32(0x52, 0x49, 0x4a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 10
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xd6, 0x00, 0x00, 255),
		C2D_Color32(0xf7, 0x28, 0x7b, 255),
		C2D_Color32(0xff, 0x69, 0x29, 255),
		C2D_Color32(0xff, 0xef, 0x5a, 255),
		C2D_Color32(0x00, 0xbe, 0xe6, 255),
		C2D_Color32(0x00, 0x71, 0xff, 255),
		C2D_Color32(0x29, 0x31, 0x94, 255),
		C2D_Color32(0xa4, 0x96, 0xff, 255),
		C2D_Color32(0x73, 0x61, 0xee, 255),
		C2D_Color32(0xce, 0xa6, 0xff, 255),
		C2D_Color32(0xad, 0x51, 0xff, 255),
		C2D_Color32(0x5a, 0x28, 0xbd, 255),
		C2D_Color32(0x4a, 0x20, 0x5a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 11
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xd6, 0x00, 0x00, 255),
		C2D_Color32(0xff, 0x00, 0x00, 255),
		C2D_Color32(0xe6, 0x96, 0x94, 255),
		C2D_Color32(0xf7, 0x28, 0x7b, 255),
		C2D_Color32(0xff, 0x69, 0xa4, 255),
		C2D_Color32(0xff, 0x51, 0xd6, 255),
		C2D_Color32(0xff, 0xaa, 0xff, 255),
		C2D_Color32(0xff, 0x69, 0x29, 255),
		C2D_Color32(0xff, 0xef, 0x5a, 255),
		C2D_Color32(0x84, 0x61, 0x42, 255),
		C2D_Color32(0xce, 0xa6, 0xff, 255),
		C2D_Color32(0xad, 0x51, 0xff, 255),
		C2D_Color32(0x4a, 0x20, 0x5a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 12
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x31, 0x31, 0x84, 255),
		C2D_Color32(0x00, 0x71, 0xff, 255),
		C2D_Color32(0x84, 0xbe, 0xff, 255),
		C2D_Color32(0x42, 0x81, 0x8c, 255),
		C2D_Color32(0x84, 0xce, 0xce, 255),
		C2D_Color32(0x00, 0xbe, 0xe6, 255),
		C2D_Color32(0xce, 0xef, 0xff, 255),
		C2D_Color32(0xad, 0x51, 0xff, 255),
		C2D_Color32(0xff, 0xef, 0x5a, 255),
		C2D_Color32(0x84, 0x61, 0x42, 255),
		C2D_Color32(0xa4, 0x96, 0xff, 255),
		C2D_Color32(0x73, 0x61, 0xee, 255),
		C2D_Color32(0x5a, 0x28, 0xbd, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 13
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x5a, 0x10, 0x00, 255),
		C2D_Color32(0xce, 0x00, 0x5a, 255),
		C2D_Color32(0xf7, 0x96, 0x7b, 255),
		C2D_Color32(0xb5, 0xce, 0x8c, 255),
		C2D_Color32(0x4a, 0x61, 0x21, 255),
		C2D_Color32(0x8c, 0xaa, 0xe6, 255),
		C2D_Color32(0x00, 0x49, 0x73, 255),
		C2D_Color32(0xbd, 0x81, 0xad, 255),
		C2D_Color32(0x9c, 0x00, 0x6b, 255),
		C2D_Color32(0xff, 0xef, 0x7b, 255),
		C2D_Color32(0xce, 0x96, 0x31, 255),
		C2D_Color32(0xff, 0x41, 0x10, 255),
		C2D_Color32(0x4a, 0x41, 0x3a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 14
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x5a, 0x69, 0x3a, 255),
		C2D_Color32(0x94, 0xb2, 0x52, 255),
		C2D_Color32(0xd6, 0xd6, 0xa4, 255),
		C2D_Color32(0xe6, 0xbe, 0x3a, 255),
		C2D_Color32(0xc5, 0xa6, 0x21, 255),
		C2D_Color32(0xbd, 0xaa, 0x94, 255),
		C2D_Color32(0x63, 0x51, 0x3a, 255),
		C2D_Color32(0xc5, 0x81, 0x84, 255),
		C2D_Color32(0x9c, 0x28, 0x19, 255),
		C2D_Color32(0xe6, 0xe7, 0x3a, 255),
		C2D_Color32(0xff, 0x79, 0x19, 255),
		C2D_Color32(0xd6, 0x51, 0x8c, 255),
		C2D_Color32(0x00, 0x96, 0xbd, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 15
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0x84, 0x39, 0x31, 255),
		C2D_Color32(0xff, 0x9e, 0xa4, 255),
		C2D_Color32(0x94, 0xb2, 0x52, 255),
		C2D_Color32(0xc5, 0xa6, 0x21, 255),
		C2D_Color32(0xde, 0xd6, 0xce, 255),
		C2D_Color32(0xce, 0xce, 0xbd, 255),
		C2D_Color32(0xc5, 0xbe, 0xad, 255),
		C2D_Color32(0xb5, 0xa6, 0x9c, 255),
		C2D_Color32(0xa4, 0x96, 0x84, 255),
		C2D_Color32(0x8c, 0x81, 0x73, 255),
		C2D_Color32(0x7b, 0x69, 0x5a, 255),
		C2D_Color32(0x73, 0x59, 0x4a, 255),
		C2D_Color32(0x63, 0x51, 0x3a, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 16
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xb5, 0x00, 0x00, 255),
		C2D_Color32(0xff, 0x9e, 0xa4, 255),
		C2D_Color32(0x31, 0xff, 0xd6, 255),
		C2D_Color32(0x5a, 0x51, 0xee, 255),
		C2D_Color32(0xe6, 0xe7, 0xe6, 255),
		C2D_Color32(0xce, 0xce, 0xce, 255),
		C2D_Color32(0xb5, 0xb2, 0xb5, 255),
		C2D_Color32(0x9c, 0x9e, 0x9c, 255),
		C2D_Color32(0x84, 0x81, 0x84, 255),
		C2D_Color32(0x63, 0x61, 0x63, 255),
		C2D_Color32(0x4a, 0x49, 0x4a, 255),
		C2D_Color32(0x31, 0x31, 0x31, 255),
		C2D_Color32(0x19, 0x18, 0x19, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 17
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xe6, 0xc6, 0x4a, 255),
		C2D_Color32(0xff, 0xef, 0x00, 255),
		C2D_Color32(0xff, 0xf7, 0x9c, 255),
		C2D_Color32(0xff, 0xf7, 0xbd, 255),
		C2D_Color32(0x52, 0xc6, 0xe6, 255),
		C2D_Color32(0xc5, 0xe7, 0xf7, 255),
		C2D_Color32(0x94, 0x79, 0x08, 255),
		C2D_Color32(0x6b, 0x69, 0x6b, 255),
		C2D_Color32(0x4a, 0x49, 0x4a, 255),
		C2D_Color32(0xf7, 0xe7, 0xee, 255),
		C2D_Color32(0x63, 0x41, 0x10, 255),
		C2D_Color32(0x94, 0x79, 0x52, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x94, 0x79, 0x08, 255),
		C2D_Color32(0x29, 0x51, 0x10, 255)
	},
	{ // 18
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xee, 0x39, 0x19, 255),
		C2D_Color32(0xff, 0xb2, 0x6b, 255),
		C2D_Color32(0x8c, 0x71, 0x10, 255),
		C2D_Color32(0x00, 0xdf, 0x19, 255),
		C2D_Color32(0x00, 0xa6, 0x19, 255),
		C2D_Color32(0x8c, 0xef, 0xff, 255),
		C2D_Color32(0x00, 0x59, 0xe6, 255),
		C2D_Color32(0x63, 0x59, 0xff, 255),
		C2D_Color32(0xc5, 0x59, 0xff, 255),
		C2D_Color32(0xff, 0xa6, 0xa4, 255),
		C2D_Color32(0x94, 0x00, 0x00, 255),
		C2D_Color32(0x9c, 0x69, 0x31, 255),
		C2D_Color32(0x84, 0x81, 0x84, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	},
	{ // 19
		C2D_Color32(0x00, 0x00, 0x00, 0),
		C2D_Color32(0xee, 0xf7, 0x9c, 255),
		C2D_Color32(0xff, 0x8a, 0xde, 255),
		C2D_Color32(0xff, 0xd6, 0xf7, 255),
		C2D_Color32(0xe6, 0xa6, 0xd6, 255),
		C2D_Color32(0xe6, 0x51, 0xb5, 255),
		C2D_Color32(0xb5, 0x28, 0x84, 255),
		C2D_Color32(0x5a, 0x10, 0x29, 255),
		C2D_Color32(0xbd, 0x41, 0x9c, 255),
		C2D_Color32(0x7b, 0x71, 0x10, 255),
		C2D_Color32(0x5a, 0x51, 0x08, 255),
		C2D_Color32(0xf7, 0xf7, 0x7b, 255),
		C2D_Color32(0x31, 0x31, 0x31, 255),
		C2D_Color32(0x94, 0x96, 0x8c, 255),
		C2D_Color32(0xff, 0xff, 0xff, 255),
		C2D_Color32(0x00, 0x00, 0x00, 255)
	}
};

int EmblemChange::getPalNumber(u8 byte, bool secondPixel) {
	if (secondPixel) {
		if ((byte & 0x0F) == 0x00) {
			return 0;
		} else if ((byte & 0x0F) == 0x01) {
			return 1;
		} else if ((byte & 0x0F) == 0x02) {
			return 2;
		} else if ((byte & 0x0F) == 0x03) {
			return 3;
		} else if ((byte & 0x0F) == 0x04) {
			return 4;
		} else if ((byte & 0x0F) == 0x05) {
			return 5;
		} else if ((byte & 0x0F) == 0x06) {
			return 6;
		} else if ((byte & 0x0F) == 0x07) {
			return 7;
		} else if ((byte & 0x0F) == 0x08) {
			return 8;
		} else if ((byte & 0x0F) == 0x09) {
			return 9;
		} else if ((byte & 0x0F) == 0x0A) {
			return 10;
		} else if ((byte & 0x0F) == 0x0B) {
			return 11;
		} else if ((byte & 0x0F) == 0x0C) {
			return 12;
		} else if ((byte & 0x0F) == 0x0D) {
			return 13;
		} else if ((byte & 0x0F) == 0x0E) {
			return 14;
		} else if ((byte & 0x0F) == 0x0F) {
			return 15;
		}
	} else {
		if ((byte & 0xF0) == 0x00) {
			return 0;
		} else if ((byte & 0xF0) == 0x10) {
			return 1;
		} else if ((byte & 0xF0) == 0x20) {
			return 2;
		} else if ((byte & 0xF0) == 0x30) {
			return 3;
		} else if ((byte & 0xF0) == 0x40) {
			return 4;
		} else if ((byte & 0xF0) == 0x50) {
			return 5;
		} else if ((byte & 0xF0) == 0x60) {
			return 6;
		} else if ((byte & 0xF0) == 0x70) {
			return 7;
		} else if ((byte & 0xF0) == 0x80) {
			return 8;
		} else if ((byte & 0xF0) == 0x90) {
			return 9;
		} else if ((byte & 0xF0) == 0xA0) {
			return 10;
		} else if ((byte & 0xF0) == 0xB0) {
			return 11;
		} else if ((byte & 0xF0) == 0xC0) {
			return 12;
		} else if ((byte & 0xF0) == 0xD0) {
			return 13;
		} else if ((byte & 0xF0) == 0xE0) {
			return 14;
		} else if ((byte & 0xF0) == 0xF0) {
			return 15;
		}
	}
	return 0;
}

u32 EmblemChange::emblemPixel(int pixel, bool secondPixel) {
	return emblemPalette[emblemData.palLine-1][getPalNumber(emblemData.sprite[pixel], secondPixel)];
}

void EmblemChange::renderEmblem(void) {
	int w = 0, h = 0, hLimit = 8;
	for (int i = 0; i < (8*8)*(8*8); i+=8) {
		emblemImage[(h*64)+w] = emblemPixel(i/2, false);
		emblemImage[(h*64)+w+1] = emblemPixel((i+1)/2, true);
		emblemImage[(h*64)+w+2] = emblemPixel((i+2)/2, true);
		emblemImage[(h*64)+w+3] = emblemPixel((i+3)/2, false);
		emblemImage[(h*64)+w+4] = emblemPixel((i+4)/2, false);
		emblemImage[(h*64)+w+5] = emblemPixel((i+5)/2, true);
		emblemImage[(h*64)+w+6] = emblemPixel((i+6)/2, true);
		emblemImage[(h*64)+w+7] = emblemPixel((i+7)/2, false);
		h++;
		if (h >= hLimit) {
			w += 8;
			if (w >= 64) {
				w = 0;
				hLimit += 8;
			}
			h = hLimit-8;
		}
	}
}

void EmblemChange::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(sprites_icon_msg_idx, 132, -2);
	if (messageNo == 3) {
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "Failed to import emblem.");
	} else if (messageNo == 2) {
		Gui::DrawStringCentered(0, 58, 0.60, BLACK, "Emblem exported successfully.");
		Gui::DrawStringCentered(0, 94, 0.60, BLACK, "You can go to \"Import Emblems\"");
		Gui::DrawStringCentered(0, 114, 0.60, BLACK, "and restore the exported emblem");
		Gui::DrawStringCentered(0, 134, 0.60, BLACK, "at any time.");
	} else if (messageNo == 1) {
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
	extern bool doubleSpeed;
	if (!doubleSpeed) {
		if (iFps > 30) {
			C3D_FrameRate(60);
		} else {
			C3D_FrameRate(iFps*2);
		}
		doubleSpeed = true;
	}

	Gui::ScreenDraw(Top);

	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(255, 255, 164, 255));
		bool dark = true;
		int x = 0;
		int width = 29;
		for (int i = 0; i < 14; i++) {
			if (dark) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(214, 247, 140, 255));
			}
			dark = !dark;
			x += width;
			width = dark ? 29 : 28;
		}
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, WHITE);	// Fill gaps of BG
		for(int w = 0; w < 7; w++) {
			for(int h = 0; h < 3; h++) {
				GFX::DrawSprite(sprites_phone_bg_idx, -72+bg_xPos+w*72, bg_yPos+h*136);
			}
		}
	}
	GFX::DrawSprite(sprites_emblem_back_idx, 100, 20, 2, 2);

	int x = 136;
	int y = 56;
	bool big = true;

	for (int h = 0; h < 64; h++) {
		for (int w = 0; w < 64; w++) {
			if (emblemImage[(h*64)+w] != 0) Gui::Draw_Rect(x+(w*(big*2)), y+(h*(big*2)), 1+big, 1+big, emblemImage[(h*64)+w]);
		}
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	C3D_FrameEnd(0);

	if (shiftBySubPixel) return;
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::ScreenDraw(Bottom);
	if (highlightedGame == 3) {
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(245, 245, 160, 255));
		bool light = true;
		int x = 0;
		int width = 17;
		for (int i = 0; i < 14; i++) {
			if (light) {
				Gui::Draw_Rect(x, 0, width, 240, C2D_Color32(255, 251, 189, 255));
			}
			light = !light;
			x += width;
			width = light ? 29 : 28;
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

	char emblemText[32];
	cursorX = 248;
	if (subScreenMode == 2) {
		cursorY = 64+(48*importEmblemList_cursorPositionOnScreen);

		// Game name
		switch (importPage) {
			case 1:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, "Your emblem files");
				break;
			case 0:
				Gui::DrawStringCentered(0, 8, 0.50, BLACK, "Savvy Manager");
				break;
		}
		Gui::DrawString(8, 8, 0.50, BLACK, "<");
		Gui::DrawString(304, 8, 0.50, BLACK, ">");

	  if (!displayNothing) {
		int i2 = (highlightedGame == 3 ? 56 : 48);
		for (int i = import_emblemShownFirst; i < import_emblemShownFirst+3; i++) {
			if (importPage == 1) {
				if (i >= numberOfExportedEmblems) break;
			} else {
				if (i > totalEmblems) break;
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
	} else if (subScreenMode == 1) {
		if (highlightedGame == 2) {
			sprintf(emblemText, "Emblem");
		} else {
			sprintf(emblemText, "Emblem %i", cursorPosition+1);
		}

		cursorY = 64+(48*emblemChangeMenu_cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, emblemText);

		int i2 = (highlightedGame == 3 ? 56 : 48);
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Import emblem");
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, BLACK, "Export emblem");
	} else {
		cursorY = 64+(48*cursorPosition);

		Gui::DrawString(8, 8, 0.50, BLACK, "Select the emblem to change.");

		int i2 = (highlightedGame == 3 ? 56 : 48);
		for (int i = 0; i <= totalEmblems; i++) {
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

	GFX::drawCursor(cursorX, cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void EmblemChange::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			showMessage = false;
		}
	} else {
		if (subScreenMode == 2) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					importEmblemList_cursorPosition--;
					importEmblemList_cursorPositionOnScreen--;
					if (importEmblemList_cursorPosition < 0) {
						importEmblemList_cursorPosition = 0;
						import_emblemShownFirst = 0;
					} else if (importEmblemList_cursorPosition < import_emblemShownFirst) {
						import_emblemShownFirst--;
					}

					if (importEmblemList_cursorPositionOnScreen < 0) {
						importEmblemList_cursorPositionOnScreen = 0;
					}
				}

				if (hDown & KEY_DOWN) {
					sndHighlight();
					importEmblemList_cursorPosition++;
					importEmblemList_cursorPositionOnScreen++;
					if (importEmblemList_cursorPosition > totalEmblems) {
						importEmblemList_cursorPosition = totalEmblems;
						import_emblemShownFirst = totalEmblems-2;
						if (import_emblemShownFirst < 0) import_emblemShownFirst = 0;
						if (importEmblemList_cursorPositionOnScreen > totalEmblems) {
							importEmblemList_cursorPositionOnScreen = totalEmblems;
						}
					} else if (importEmblemList_cursorPosition > import_emblemShownFirst+2) {
						import_emblemShownFirst++;
					}

					if (importEmblemList_cursorPositionOnScreen > 2) {
						importEmblemList_cursorPositionOnScreen = 2;
					}
				}
			}

			if (hDown & KEY_A) {
				bool exportFound = false;
				if (importPage == 1 && totalEmblems > 0) {
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(importEmblemList_cursorPosition));
							if (access(embFilePath, F_OK) == 0) {
								sndSelect();
								readSS4EmblemFile(cursorPosition, embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/%s.emb", getExportedEmblemName(importEmblemList_cursorPosition));
							if (access(embFilePath, F_OK) == 0) {
								sndSelect();
								readSS3EmblemFile(embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(emblemImported, "Imported %s successfully.", getExportedEmblemName(importEmblemList_cursorPosition));
						messageNo = 1;
						subScreenMode = 1;
					} else {
						sndBack();
						messageNo = 3;
					}
					showMessage = true;
					} else if (importPage == 0) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[importEmblemList_cursorPosition]);
							if (access(embFilePath, F_OK) == 0) {
								readSS4EmblemFile(cursorPosition, embFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(embFilePath, "romfs:/emblems/%s.emb", import_emblemNames[importEmblemList_cursorPosition]);
							if (access(embFilePath, F_OK) == 0) {
								readSS3EmblemFile(embFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(emblemImported, "Imported %s successfully.", import_emblemNames[importEmblemList_cursorPosition]);
						messageNo = 1;
						subScreenMode = 1;
					} else {
						sndBack();
						messageNo = 3;
					}
					showMessage = true;
				}
			}

			if (hDown & KEY_LEFT) {
				sndHighlight();
				importPage--;
				if (importPage < 0) importPage = 1;
				importEmblemList_cursorPosition = 0;
				importEmblemList_cursorPositionOnScreen = 0;
				import_emblemShownFirst = 0;
				getMaxEmblems();
			}

			if (hDown & KEY_RIGHT) {
				sndHighlight();
				importPage++;
				if (importPage > 1) importPage = 0;
				importEmblemList_cursorPosition = 0;
				importEmblemList_cursorPositionOnScreen = 0;
				import_emblemShownFirst = 0;
				getMaxEmblems();
			}

			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				subScreenMode = 1;
			}
		} else if (subScreenMode == 1) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					emblemChangeMenu_cursorPosition--;
					if (emblemChangeMenu_cursorPosition < 0) {
						emblemChangeMenu_cursorPosition = 0;
					}
				}

				if (hDown & KEY_DOWN) {
					sndHighlight();
					emblemChangeMenu_cursorPosition++;
					if (emblemChangeMenu_cursorPosition > 1) {
						emblemChangeMenu_cursorPosition = 1;
					}
				}
			}

			if (hDown & KEY_A) {
				if (emblemChangeMenu_cursorPosition == 1) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem %i.chr", cursorPosition);
							writeSS4EmblemFile(cursorPosition, embFilePath);
							break;
						case 2:
							sprintf(embFilePath, "sdmc:/3ds/SavvyManager/emblems/Emblem.chr");
							writeSS3EmblemFile(embFilePath);
							break;
					}
					messageNo = 2;
					showMessage = true;
				} else {
					sndSelect();
					subScreenMode = 2;
					displayNothing = true;
					gspWaitForVBlank();
					getExportedEmblemContents();
					getMaxEmblems();
					displayNothing = false;
				}
			}
			if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
				sndBack();
				subScreenMode = 0;
				getMaxEmblems();
			}
		} else {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					cursorPosition--;
					if (cursorPosition < 0) {
						cursorPosition = 0;
					}
					if (highlightedGame == 3) {
						readSS4Emblem(cursorPosition);
						renderEmblem();
					}
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					cursorPosition++;
					if (cursorPosition > totalEmblems) {
						cursorPosition = totalEmblems;
					}
					if (highlightedGame == 3) {
						readSS4Emblem(cursorPosition);
						renderEmblem();
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
	}
}