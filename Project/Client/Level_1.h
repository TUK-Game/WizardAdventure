#pragma once
#include <Engine/Level.h>

class CLevel_1
	: public CLevel
{
public:
	CLevel_1();
	~CLevel_1();

public:
	virtual void Init();
	virtual void Begin();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void End();

public:
	virtual CLevel_1* Clone() override { return new CLevel_1(*this); }

public:
	class CGameObject* m_MiniMapBackground;
};



