#pragma once

class CNetworkManager :
	public CSingleton<CNetworkManager>
{
	DECLARE_SINGLETON(CNetworkManager)

public:
	void PushJob(std::function<void()>&& callback)
	{
		std::shared_ptr<CJob> job = std::make_shared<CJob>(std::move(callback));
		m_JobQueue.Push(job);
	}

	template<typename T, typename Ret, typename... Args>
	void Push(std::shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args... args)
	{
		std::shared_ptr<CJob> job = std::make_shared<CJob>(owner, memFunc, std::forward<Args>(args)...);
		m_JobQueue.Push(job);
	}

public:
	bool Init();
	void ExecuteJobs();

private:
	bool InitHost();
	bool InitClient();

public:
	static std::shared_ptr<class CServerSession>	s_GameSession;	// Client로 로그인할 경우 서버 세션

private:
	CLockQueue<std::shared_ptr<CJob>>		m_JobQueue;
};

