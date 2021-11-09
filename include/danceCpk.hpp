#ifndef DANCECPK_H
#define DANCECPK_H


#include <3ds.h>

extern void readDanceCpk(void);
extern int danceGetPeopleAmount(const int video);
extern void danceSetPeopleAmount(const int video, int amount);
extern u16 danceGetCharacterId(const int video, const int slot);
extern void danceWriteDataToCpk(const int video);

#endif // DANCECPK_H
