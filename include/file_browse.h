#ifndef FILE_BROWSE_H
#define FILE_BROWSE_H

#ifdef __cplusplus
extern "C" {
#endif

extern int numberOfExportedCharacters;
extern int numberOfExportedEmblems;
extern int numberOfMusicPacks;

void getExportedCharacterContents (void);
const char* getExportedCharacterName (int num);
bool getExportedCharacterGender (int num);

void getExportedEmblemContents (void);
const char* getExportedEmblemName (int num);

void getMusicPackContents (void);
const char* getMusicPackName (int num);

#ifdef __cplusplus
}
#endif

#endif // FILE_BROWSE_H
