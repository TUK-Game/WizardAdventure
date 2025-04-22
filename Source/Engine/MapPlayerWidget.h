#pragma once
#include "ImageWidget.h"

class CMapPlayerWidget :
	public CImageWidget
{
public:
	CMapPlayerWidget();
	virtual ~CMapPlayerWidget();

public:
	virtual void Update() override;

public:

	bool InitPlayer(class CPlayer* player, int idx);
	void ConvertPositionToMap();

public:
	virtual CMapPlayerWidget* Clone() override { return new CMapPlayerWidget(*this); }

private:
	CGameObject* m_OwnerPlayer;

	Vec3 m_PlayerPosition;
	Vec2 m_MapMax;
	Vec2 m_MapMin;
};

