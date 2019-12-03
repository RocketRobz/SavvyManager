#ifndef SAVEDATA_H
#define SAVEDATA_H


#ifdef __cplusplus
extern "C" {
#endif

#include <3ds.h>

#define ss2SavePath	"sdmc:/3ds/Checkpoint/saves/0x00A91 Style Savvy  Trendsetters/SavvyManager/record_card.bin"
#define ss3SavePath	"sdmc:/3ds/Checkpoint/saves/0x01965 Style Savvy  Fashion Forward/SavvyManager/savedata.dat"
#define ss4SavePath	"sdmc:/3ds/Checkpoint/extdata/0x01C25 Style Savvy  Styling Star/SavvyManager/savedata.dat"

typedef struct ss2character {
    u8 gender;				// Gender (01=Female, 02=Male)
    u8 poseSet;				// Pose set (01=Normal, 02=Cute, 03=Bold)
    u8 height;				// Height (00=Character is invisible)
    u8 skinColor;
    u8 faceShape;			// Face shape (03=Left, 01=Mid, 02=Right)
    u8 eyes;
    u8 eyebrows;
    u8 mouthShape;
    u8 unknown8;
    u8 hairStyle;
    u8 hairColorMain;
    u8 hairColorHighlights;
    u16 eyelinerColor;
    u16 contactColor;
    u16 eyeShadowColor;
    u16 eyeColor;
    u16 lipColor;
    u16 blush;
    u8 mascaraLength;
    u8 blushPosition;
    u16 inner;
    u16 top;
    u16 outer;
    u16 scarf;
    u16 gloves;
    u16 necklace;
    u16 socks;				// Socks/Leggings/Tights
    u16 pants;
    u16 skirt;
    u16 shoes;
    u16 legWarmers;
    u16 hat;
    u16 glasses;
    u16 dress;				// Dress/Jumpsuit
    u16 bag;
} ss2character;

typedef struct ss3to4character {
    u8 gender;				// Gender (01=Female, 02=Male)
    u8 height;				// Height (00=Character is invisible)
    u8 poseSet;				// Pose set (01=Normal, 02=Cute, 03=Bold)
    u8 skinColor;
    u8 faceShape;			// Face shape (03=Left, 01=Mid, 02=Right)
    u8 eyes;
    u8 mouthShape;
    u8 eyebrows;
    u8 faceMole;			// Face mole (Female), Facial hair (Male)
    u8 freckles;			// Unknown for Styling Star
    u8 mascaraColor;
    u8 unknownC;
    u8 unknownD;
    u8 contactColor;
    u8 unknownF;
    u8 mascaraLength;
    u8 unknown11;
    u8 lipShine;
    u8 eyeColor;
    u8 eyebrowOverlay;
    u8 hairStyle;
    u8 hairFringe;
    u8 hairColorMain;
    u8 hairColorHighlights;
    u8 expessFreckles;		// Show expressions for Fashion Forward, or freckles for Styling Star
    u8 nailColor;
    u8 eyeliner;			// Eyeliner (00=Normal, 01=Double)
    u8 unknown1C;
    u8 unknown1D;
    u16 inner;
    u16 top;				// Top/Dress/Yukatas (Female), Yukatas/Dungarees (Male)
    u16 outer;
    u16 necklaceScarf;		// Necklace/Scarf
    u16 gloves;
    u16 socks;				// Socks/Leggings/Tights
    u16 skirtPants;			// Skirt/Pants
    u16 shoes;
    u16 legWarmers;
    u16 hat;
    u16 glasses;
    u16 bag;
    u16 unknown36;
    u16 unknown38;
    u16 unknown3A;
    u8 bowPosition;
    u8 unknown3D;
} ss3to4character;

//extern ss3to4character ss4CharacterData;

//extern char ss3Save[0x174000];
//extern char ss4Save[0xF0000];

extern char ss2PlayerName[10];
extern char ss3PlayerName[10];
extern char ss4PlayerName[10];

extern void readSS2Save(void);
extern void writeSS2Save(void);
extern void readSS3Save(void);
extern void writeSS3Save(void);
extern void readSS4Save(void);
extern void writeSS4Save(void);

extern void readSS2Character(void);
extern void writeSS2Character(void);
extern void readSS3Character(u16 id);
extern void writeSS3Character(u16 id);
extern void readSS4Character(u16 id);
extern void writeSS4Character(u16 id);

extern void readSS2CharacterFile(const char* filename);
extern void writeSS2CharacterFile(const char* filename);
extern void readSS3CharacterFile(u16 id, const char* filename);
extern void writeSS3CharacterFile(u16 id, const char* filename);
extern void readSS4CharacterFile(u16 id, const char* filename);
extern void writeSS4CharacterFile(u16 id, const char* filename);

extern bool getSS2CharacterGender(void);
extern bool getSS3CharacterGender(u16 id);
extern bool getSS4CharacterGender(u16 id);

#ifdef __cplusplus
}
#endif

#endif // SAVEDATA_H
