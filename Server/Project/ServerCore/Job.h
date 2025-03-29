#pragma once

class CJob
{
public:
	CJob(std::function<void()>&& callback) : m_Callback(std::move(callback))
	{
	}

	template<typename T, typename Ret, typename... Args>
	CJob(std::shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		m_Callback = [owner, memFunc, args...]()
		{
			(owner.get()->*memFunc)(args...);
		};
	}

	void Execute()
	{
		m_Callback();
	}

private:
	std::function<void()> m_Callback;
};

