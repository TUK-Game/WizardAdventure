#pragma once
#include <Engine/Level.h>

class CLevel_Start
	: public CLevel
{
public:
	CLevel_Start();
	~CLevel_Start();

public:
	virtual void Init();
	virtual void Begin();
	virtual void Update();
	virtual void FinalUpdate();
	virtual void End();

public:
	virtual CLevel_Start* Clone() override { return new CLevel_Start(*this); }

public:
};



