#include <3ds.h>
#include "thread.h"

void createThread(ThreadFunc entrypoint) {
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	threadCreate((ThreadFunc)entrypoint, NULL, 4*1024, prio-1, -2, true);
}
