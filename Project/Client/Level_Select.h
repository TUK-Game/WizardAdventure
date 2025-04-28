#pragma once
#include <Engine/Level.h>

class CLevel_Select
	: public CLevel
{
public:
	CLevel_Select();
	~CLevel_Select();

public:
	virtual void Init();
	virtual void Begin();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void End();

public:
	virtual CLevel_Select* Clone() override { return new CLevel_Select(*this); }

};

