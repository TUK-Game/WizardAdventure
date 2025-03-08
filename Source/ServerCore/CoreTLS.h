#pragma once
#include <stack>

extern thread_local uint32				tls_ThreadId;
extern thread_local uint64				tls_EndTickCount;
extern thread_local class CJobQueue*	tls_CurrentJobQueue;