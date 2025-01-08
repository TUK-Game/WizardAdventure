#pragma once

#include <string>

template <typename T>
class CSharedPtr
{
public:
	CSharedPtr() {}
	CSharedPtr(T* ptr)						{ Set(ptr); };

	CSharedPtr(const CSharedPtr& rhs)		{ Set(rhs.m_ptr); }
	CSharedPtr(CSharedPtr&& rhs) noexcept	{ m_ptr = rhs.m_ptr; rhs.m_ptr = nullptr; }
	// 상속 관계 복사
	template <typename U>
	CSharedPtr(CSharedPtr<U>& rhs)			{ Set(static_cast<T*>(rhs.m_ptr)); }

	~CSharedPtr() { Release(); }

public:
	// 복사 연산자
	CSharedPtr& operator=(const CSharedPtr& rhs)
	{
		if (m_ptr != rhs.m_ptr)
		{
			Release();
			Set(rhs.m_ptr);
		}
		return *this;
	}

	// 이동 연산자
	CSharedPtr& operator=(CSharedPtr&& rhs) noexcept
	{
		Release();
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
		return *this;
	}

	bool operator==(const CSharedPtr& rhs) const	{ return m_ptr == rhs.m_ptr; }
	bool operator==(T* ptr) const					{ return m_ptr == ptr; }
	bool operator!=(const CSharedPtr& rhs) const	{ return m_ptr != rhs.m_ptr; }
	bool operator!=(T* ptr) const					{ return m_ptr != ptr; }
	bool operator<(const CSharedPtr& rhs) const		{ return m_ptr < rhs.m_ptr; }
	T* operator*()									{ return m_ptr; }
	const T* operator*() const						{ return m_ptr; }
	operator T* () const							{ return m_ptr; }
	T* operator->()									{ return m_ptr; }
	const T* operator->() const						{ return m_ptr; }

	bool IsNull()									{ return m_ptr == nullptr; }

public:
	inline void Set(T* ptr)
	{
		m_ptr = ptr;
		if (ptr)
			ptr->AddRef();
	}

	inline void Release()
	{
		if (m_ptr)
		{
			m_ptr->ReleaseRef();
			m_ptr = nullptr;
		}
	}

private:
	T* m_ptr = nullptr;
};