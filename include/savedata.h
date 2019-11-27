#ifndef SAVEDATA_H
#define SAVEDATA_H


#ifdef __cplusplus
extern "C" {
#endif

#include <3ds.h>

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
    u8 eyeMakeupColor;
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
    u16 unknown2E;
    u16 hat;
    u16 glasses;
    u16 bow;
    u16 unknown36;
    u16 unknown38;
    u16 unknown3A;
    u8 bowPosition;
    u8 unknown3D;
} ss3to4character;

extern ss3to4character ss4CharacterData;

extern char ss3Save[0x174000];
extern char ss4Save[0xF0000];

extern char ss4PlayerName[10];

extern void readSS4Save(void);
extern void writeSS4Save(void);

extern void readSS4Character(u16 id);
extern void writeSS4Character(u16 id);

extern void readSS4CharacterFile(u16 id, const char* filename);

extern bool getSS4CharacterGender(u16 id);

#ifdef __cplusplus
}
#endif

#endif // SAVEDATA_H
