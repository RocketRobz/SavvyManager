#ifndef FILE_BROWSE_H
#define FILE_BROWSE_H

#ifdef __cplusplus
extern "C" {
#endif

extern int numberOfMusicPacks;

void getMusicPackContents (void);
const char* getMusicPackName (int num);

#ifdef __cplusplus
}
#endif

#endif // FILE_BROWSE_H
