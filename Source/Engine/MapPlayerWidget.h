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
	bool InitPlayer();
	void ConvertPositionToMap();

public:
	virtual CMapPlayerWidget* Clone() override { return new CMapPlayerWidget(*this); }

private:
	CGameObject* m_OwnerPlayer;

	float m_AspectRatio;
	Vec3 m_PlayerPosition;
	Vec3 m_MapSize;
	Vec2 m_MapMax;
	Vec2 m_MapMin;
	Vec3 m_MapCenter;
	Vec3 m_Offset;
};

