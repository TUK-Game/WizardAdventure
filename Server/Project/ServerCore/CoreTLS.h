#pragma once
#include <stack>

extern thread_local uint32				tl_ThreadId;
extern thread_local uint64				tl_EndTickCount;
extern thread_local class CJobQueue*	tl_CurrentJobQueue;