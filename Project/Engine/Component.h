#pragma once

class CComponent :
	public CRef
{
	friend class CGameObject;

public:
	CComponent(EComponent_Type	type);
	~CComponent();

public:
	EComponent_Type GetType() const	{ return m_Type; }
	CGameObject* GetOwner() const	{ return m_Owner; }

public:
	virtual void Begin() {};
	virtual void Update() {};
	virtual void FinalUpdate() = 0;

public:
	virtual CComponent* Clone() = 0;

private:
	EComponent_Type	m_Type;
	CGameObject*	m_Owner;
};

