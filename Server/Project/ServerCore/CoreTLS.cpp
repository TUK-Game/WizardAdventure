#include "pch.h"
#include "CoreTLS.h"

thread_local uint32				tl_ThreadId = 0;
thread_local uint64				tl_EndTickCount = 0;
thread_local std::stack<int32>	tls_LockStack;
thread_local CJobQueue*			tl_CurrentJobQueue = nullptr;