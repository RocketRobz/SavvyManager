#ifndef SAVEDATA_H
#define SAVEDATA_H


#ifdef __cplusplus
extern "C" {
#endif

#include <3ds.h>

#define ss2SavePath	"ss2:/record_card.bin"
#define ss3SavePath	"ss3:/savedata.dat"
#define ss4SavePath	"/savedata.dat"

typedef struct ss2character {
    u8 gender;				// Gender (01=Female, 02=Male)
    u8 poseSet;				// Pose set (01=Active, 02=Cute, 03=Cool)
    u8 height;				// Height (00=Character is invisible)
    u8 skinColor;
    u8 faceShape;			// Face shape (03=Left, 01=Mid, 02=Right)
    u8 eyes;
    u8 eyebrows;
    u8 mouthShape;
    u8 facialHair;			// Facial hair (Male)
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
    u8 poseSet;				// Pose set (01=Active, 02=Cute, 03=Cool)
    u8 skinColor;
    u8 faceShape;			// Face shape (03=LeftF, 01=MidF, 02=RightF, 04=LeftM, 05=MidM, 06=RightM)
    u8 eyes;
    u8 mouthShape;
    u8 eyebrows;
    u8 facialHair;			// Facial hair (Male)
    u8 faceMole;			// Face mole (Female)
    u8 lipstickColor;
    u8 mascaraColor;
    u8 eyeshadowColor;
    u8 eyelinerColor;
    u8 contactColor;
    u8 blusherColor;
    u8 mascaraLength;
    u8 blusherPosition;
    u8 lipShine;
    u8 eyeColor;
    u8 eyebrowOverlay;
    u8 hairStyle;
    u8 hairFringe;
    u8 hairColorMain;
    u8 hairColorHighlights;
    u8 freckles;
    u8 nailColor;			// Fashion Forward: Facial expression (00=Default), Styling Star: Nail color
    u8 eyeliner;			// Fashion Forward: 01 = No facial expressions, Styling Star: Eyeliner (00=Normal, 01=Double)
    u8 unknown1C;
    u8 eyebrowColor;		// Fashion Forward: Eyebrow color, Styling Star: Unknown
    u16 inner;
    u16 top;				// Top/Dress/Yukatas (Female), Yukatas/Dungarees (Male)
    u16 outer;
    u16 necklaceScarfHeadphones;		// Fashion Forward: Necklace/Scarf, Styling Star: Scarf/Headphones
    u16 gloves;
    u16 socks;				// Socks/Leggings/Tights
    u16 skirtPants;			// Skirt/Pants
    u16 shoes;
    u16 legWarmers;
    u16 hat;
    u16 glasses;			// Also Earrings in Fashion Forward
    u16 bag;
    u16 necklace;			// Styling Star
    u16 watch;				// Styling Star
    u16 earrings;			// Styling Star
    u8 bowPosition;			// Styling Star
    u8 unknown3D;
} ss3to4character;

typedef struct ss3to4emblem {
    u32 palLine;
	u8 sprite[0x800];
} ss3to4emblem;

extern u8 saveRegion[4];

//extern ss3to4character ss4CharacterData;

extern ss3to4emblem emblemData;

extern const char* ss1SavePath;

extern char ss2PlayerName[10];
extern char ss3PlayerName[10];
extern char ss4PlayerName[10];

extern Handle handle4;
extern FS_Archive archive4;

extern void commitSaveData(void);

extern void readSS2Save(void);
extern void writeSS2Save(void);
extern void readSS3Save(void);
extern void writeSS3Save(void);
extern bool foundSS4Save(void);
extern void readSS4Save(void);
extern void writeSS4Save(void);

extern void readSS2Character(void);
extern void writeSS2Character(void);
extern void readSS3Character(u16 id);
extern void readSS4CharacterToSS3(u16 id);
extern void writeSS3Character(u16 id);
extern void readSS4Character(u16 id);
extern void readSS3CharacterToSS4(u16 id);
extern void writeSS4Character(u16 id);

extern void toggleSS3Character(u16 id, bool enable);
extern bool existsSS3Character(u16 id);
extern void removeSS3CharacterBag(u16 id);
extern bool existsSS4Character(u16 id);

extern void readSS2CharacterFile(const char* filename);
extern void writeSS2CharacterFile(const char* filename);
extern void writeSS2CharacterToSave(void);
extern void readSS3CharacterFile(u16 id, const char* filename);
extern void writeSS3CharacterFile(u16 id, const char* filename);
extern void writeSS3CharacterToSave(u16 id);
extern void readSS4CharacterFile(u16 id, const char* filename);
extern void writeSS4CharacterFile(u16 id, const char* filename);
extern void writeSS4CharacterToSave(u16 id);
extern void writeSS4AssistantCharacterToSave(void);
extern void writeSS4MewtubeCharacter(int video, int slot);
extern void writeSS4MewtubeCharacterToSave(int video, int slot);
extern void revertSS4MewtubeCharacter(int video, int slot);
extern void backupSS3DLCharacters(const char* filename);
extern void restoreSS3DLCharacters(const char* filename);

extern bool getSS2CharacterGender(void);
extern bool getSS2CharacterGenderNoExceptions(void);
extern bool getSS3CharacterGender(u16 id);
extern bool getSS3CharacterGenderNoExceptions(u16 id);
extern bool getSS4CharacterGender(u16 id);
extern bool getSS4CharacterGenderNoExceptions(u16 id);
extern void changeSS4CharacterGender(u16 id);
extern u16 getSS4AssistantCharacterId(void);
extern void writeSS4AssistantCharacterId(u16 id);
extern u16 getSS4MewtubeCharacterId(int video, int slot);
extern void writeSS4MewtubeCharacterId(u16 id, int video, int slot);

extern u8 readSS2CharacterHeight(void);
extern void writeSS2CharacterHeight(u8 height);
extern u8 readSS3CharacterHeight(u16 id);
extern void writeSS3CharacterHeight(u16 id, u8 height);
extern u8 readSS4CharacterHeight(u16 id);
extern void writeSS4CharacterHeight(u16 id, u8 height);

extern u8 readSS2CharacterPoseSet(void);
extern void writeSS2CharacterPoseSet(u8 poseSet);
extern u8 readSS3CharacterPoseSet(u16 id);
extern void writeSS3CharacterPoseSet(u16 id, u8 poseSet);
extern u8 readSS4CharacterPoseSet(u16 id);
extern void writeSS4CharacterPoseSet(u16 id, u8 poseSet);

extern const char* readSS3ProfileName(u16 id);
extern void readSS3ProfileFile(u16 id, const char* filename);
extern void writeSS3ProfileFile(u16 id, const char* filename);
extern const char* readSS4ProfileName(u16 id);

extern void readSS3Emblem(void);
extern void writeSS3Emblem(void);
extern void writeSS3EmblemToSave(void);
extern void readSS4Emblem(int id);
extern void writeSS4Emblem(int id);
extern void writeSS4EmblemToSave(int id);

void readSS3EmblemFile(const char* filename);
void writeSS3EmblemFile(const char* filename);
void readSS4EmblemFile(int id, const char* filename);
void writeSS4EmblemFile(int id, const char* filename);
void readTempEmblemFile(const char* filename);

extern u16 readSS3FashionOwnFlag(int id);
extern void writeSS3FashionOwnFlag(int id, u16 flag);
extern u8 readSS3FashionOwnWardFlag(int id);
extern void writeSS3FashionOwnWardFlag(int id, u8 flag);
extern void writeSS3FashionOwnFlagsToSave(void);
extern u16 readSS4FashionOwnFlag(int id);
extern void writeSS4FashionOwnFlag(int id, u16 flag);
extern u8 readSS4FashionOwnWardFlag(int id);
extern void writeSS4FashionOwnWardFlag(int id, u8 flag);
extern void writeSS4FashionOwnFlagsToSave(void);

#ifdef __cplusplus
}
#endif

#endif // SAVEDATA_H
