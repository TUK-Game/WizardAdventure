#pragma once

#include "GameObject.h"

class CTransform;
class CMeshRenderer;
class CCamera;


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

protected:
	CTransform* GetTransform()			{ return m_Owner->GetTransform(); }
	CMeshRenderer* GetMeshRenderer()	{ return m_Owner->GetMeshRenderer(); }
	CCamera* GetCamera()				{ return m_Owner->GetCamera(); }

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

