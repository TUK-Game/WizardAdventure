#pragma once
#include "ImageWidget.h"

class CMapPlayerWidget :
	public CImageWidget
{
public:
	CMapPlayerWidget();
	virtual ~CMapPlayerWidget();

public:
	virtual bool Init() override;
	virtual void Update() override;

public:
	void ConvertPositionToMap();

public:
	virtual CMapPlayerWidget* Clone() override { return new CMapPlayerWidget(*this); }

private:
	CGameObject* m_OwnerPlayer;

	float m_AspectRatio;
	Vec3 m_PlayerPosition;
	Vec3 m_MapSize;
	Vec3 m_MapCenter;
	Vec3 m_Offset;
};

