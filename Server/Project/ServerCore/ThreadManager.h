#pragma once

#include <thread>
#include <functional>

class CThreadManager
{
public:
	CThreadManager();
	~CThreadManager();

	void	Launch(std::function<void(void)> callback);
	void	Join();

	static void InitTLS();
	static void DestroyTLS();

	static void DoGlobalQueueWork();
	static void DistributeReservedJobs();

private:
	std::mutex					m_Lock;
	std::vector<std::thread>	m_vecThread;
};

