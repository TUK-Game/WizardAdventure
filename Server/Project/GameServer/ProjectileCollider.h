#pragma once
#include "BoxCollider.h"
class CProjectileCollider :
	public CBoxCollider
{
public:
	virtual void Update() override;
	virtual void SetBoxInfo(const Vec3& centerPos, const Vec3& size, const Vec3& rotation, const Vec3& offset = Vec3(0.f, 0.f, 0.f)) override;
};

