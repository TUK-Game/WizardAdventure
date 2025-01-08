#pragma once

class CLayer;


class CLevel :
	public CRef
{
public:
	CLevel();
	~CLevel();

public:
	void Begin();
	void Update();
	void FinalUpdate();

public:
	virtual CLevel* Clone() override	{ return new CLevel(*this); }

private:
	std::array<CLayer*, MAX_LAYER>	m_Layer;
};

