#include "pch.h"
#include "CoreGlobal.h"
#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"

CThreadManager* g_ThreadManager = nullptr;
CGlobalQueue* g_GlobalQueue = nullptr;
CJobTimer* g_JobTimer = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		g_ThreadManager = new CThreadManager();
		g_GlobalQueue = new CGlobalQueue();
		g_JobTimer = new CJobTimer();
		CSocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete g_ThreadManager;
		delete g_GlobalQueue;
		delete g_JobTimer;
		CSocketUtils::Clear();
	}
} GCoreGlobal;