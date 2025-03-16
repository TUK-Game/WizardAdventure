#pragma once

#include <atomic>

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

public:
	int GetRefCount() const					{ return m_RefCount; }
	std::wstring GetName() const			{ return m_Name; }
	std::wstring GetTypeName() const		{ return m_TypeName; }
	size_t GetTypeID() const				{ return m_TypeID; }
	bool GetEnable() const					{ return m_bEnable; }
	bool GetActive() const					{ return m_bActive; }

	void SetName(const std::wstring& name)	{ m_Name = name; }
	void SetEnable(bool bEnable)			{ m_bEnable = bEnable; }
	void SetActive(bool bEnable)			{ m_bActive = bEnable; }

	template <typename T>
	void SetTypeID()
	{
		m_TypeName = typeid(T).name();
		m_TypeID = typeid(T).hash_code();
	}

public:
	void AddRef();
	int ReleaseRef();

	virtual void Destroy();

	// 자기자신의 복제객체를 반환해주는 함수
	virtual CRef* Clone() = 0;

private:
	std::atomic<int>	m_RefCount;
	std::wstring		m_Name;
	std::wstring		m_TypeName;
	size_t				m_TypeID;
	bool				m_bEnable;
	bool				m_bActive;
};

