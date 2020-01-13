#include <3ds.h>
#include <vector>
#include "thread.h"

static std::vector<Thread> threads;

void createThread(ThreadFunc entrypoint) {
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 4*1024, prio-1, -2, false);
	threads.push_back(thread);
}

void destroyThreads(void) {
	for (u32 i = 0; i < threads.size(); i++)
	{
		threadJoin(threads.at(i), U64_MAX);
		threadFree(threads.at(i));
	}
}